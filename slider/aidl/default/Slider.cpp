/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Slider.h"

#include <android-base/logging.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

namespace {
constexpr char kInputDir[] = "/dev/input";
constexpr size_t kBitsPerWord = sizeof(unsigned long) * 8;

struct DeathCookie {
    Slider* slider;
};
}  // namespace

Slider::Slider(int absCode, const std::optional<SliderLocation>& location)
    : mAbsCode(absCode), mLocation(location) {
    mDeathRecipient =
            ndk::ScopedAIBinder_DeathRecipient(AIBinder_DeathRecipient_new(&onCallbackDied));
    AIBinder_DeathRecipient_setOnUnlinked(mDeathRecipient.get(), &onCookieUnlinked);
}

bool Slider::start(const std::vector<std::string>& inputNames, bool verbose) {
    // A code outside the axis range would index absBits out of bounds.
    if (mAbsCode < 0 || mAbsCode > ABS_MAX) {
        LOG(ERROR) << "Axis code " << mAbsCode << " is not an absolute axis";
        return false;
    }

    openDevice(inputNames, verbose);
    if (mFd < 0) {
        return false;
    }

    if (!startEventLoop()) {
        close(mFd);
        mFd = -1;
        return false;
    }

    return true;
}

Slider::~Slider() {
    if (mStopFd >= 0) {
        uint64_t one = 1;
        TEMP_FAILURE_RETRY(write(mStopFd, &one, sizeof(one)));
    }
    if (mThread.joinable()) {
        mThread.join();
    }

    // A notification already in flight is not waited for.
    {
        std::lock_guard<std::mutex> lock(mMutex);
        for (const auto& registration : mRegistrations) {
            AIBinder_unlinkToDeath(registration.binder.get(), mDeathRecipient.get(),
                                   registration.cookie);
        }
        mRegistrations.clear();
    }

    if (mEpollFd >= 0) {
        close(mEpollFd);
    }
    if (mStopFd >= 0) {
        close(mStopFd);
    }
    if (mFd >= 0) {
        close(mFd);
    }
}

void Slider::openDevice(const std::vector<std::string>& inputNames, bool verbose) {
    DIR* dir = opendir(kInputDir);
    if (dir == nullptr) {
        PLOG(ERROR) << "Failed to open " << kInputDir;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strncmp(entry->d_name, "event", 5) != 0) {
            continue;
        }

        std::string path = std::string(kInputDir) + "/" + entry->d_name;
        int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
        if (fd < 0) {
            continue;
        }

        char name[256] = {};
        if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), name) < 0 ||
            std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end()) {
            close(fd);
            continue;
        }

        unsigned long absBits[(ABS_CNT + kBitsPerWord - 1) / kBitsPerWord] = {};
        if (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absBits)), absBits) < 0 ||
            !(absBits[mAbsCode / kBitsPerWord] >> (mAbsCode % kBitsPerWord) & 1)) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " does not report abs code " << mAbsCode;
            }
            close(fd);
            continue;
        }

        struct input_absinfo absinfo;
        if (ioctl(fd, EVIOCGABS(mAbsCode), &absinfo) < 0) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " has no abs code " << mAbsCode;
            }
            close(fd);
            continue;
        }

        const int count = absinfo.maximum - absinfo.minimum + 1;
        if (count < 2) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " reports " << count
                           << " slider positions";
            }
            close(fd);
            continue;
        }

        // An axis that does not begin at zero still reports positions from zero.
        const int position = absinfo.value - absinfo.minimum;
        if (position < 0 || position >= count) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " rests at " << absinfo.value << ", outside ["
                           << absinfo.minimum << ", " << absinfo.maximum << "]";
            }
            close(fd);
            continue;
        }

        mFd = fd;
        mCount = count;
        mMinimum = absinfo.minimum;
        mPosition.store(position);
        LOG(INFO) << "Opened slider " << name << " (" << path << "), positions=" << mCount
                  << ", at " << position;
        break;
    }

    closedir(dir);

    if (mFd < 0 && verbose) {
        LOG(ERROR) << "No matching slider input device found";
    }
}

bool Slider::startEventLoop() {
    mStopFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (mStopFd < 0) {
        PLOG(ERROR) << "Failed to create the shutdown eventfd";
        return false;
    }

    mEpollFd = epoll_create1(EPOLL_CLOEXEC);
    if (mEpollFd < 0) {
        PLOG(ERROR) << "epoll_create1 failed";
        close(mStopFd);
        mStopFd = -1;
        return false;
    }

    struct epoll_event inputEvent = {.events = EPOLLIN, .data = {.fd = mFd}};
    struct epoll_event stopEvent = {.events = EPOLLIN, .data = {.fd = mStopFd}};
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mFd, &inputEvent) < 0 ||
        epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mStopFd, &stopEvent) < 0) {
        PLOG(ERROR) << "epoll_ctl failed";
        close(mEpollFd);
        mEpollFd = -1;
        close(mStopFd);
        mStopFd = -1;
        return false;
    }

    mThread = std::thread(&Slider::eventLoop, this);
    return true;
}

void Slider::eventLoop() {
    while (true) {
        struct epoll_event events[2];
        int n = TEMP_FAILURE_RETRY(epoll_wait(mEpollFd, events, 2, -1));
        if (n < 0) {
            PLOG(ERROR) << "epoll_wait failed";
            return;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == mStopFd) {
                return;
            }
            // Serving the last position would tell a client nothing is
            // wrong, so the process goes and every client is told.
            if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                LOG(ERROR) << "Slider input device is gone";
                _exit(EXIT_FAILURE);
            }
        }

        // After SYN_DROPPED the queue describes a slider that has already
        // moved on, so it is drained and the axis is asked where it rests.
        bool dropped = false;
        struct input_event ev;
        ssize_t count;
        while ((count = TEMP_FAILURE_RETRY(read(mFd, &ev, sizeof(ev)))) == sizeof(ev)) {
            if (ev.type == EV_SYN && ev.code == SYN_DROPPED) {
                LOG(WARNING) << "Dropped slider events, re-reading the axis";
                dropped = true;
                continue;
            }
            if (dropped || ev.type != EV_ABS || ev.code != mAbsCode) {
                continue;
            }

            const int position = ev.value - mMinimum;
            if (position < 0 || position >= mCount) {
                LOG(ERROR) << "Ignoring value " << ev.value << " outside the axis range";
                continue;
            }

            setPosition(position);
        }

        if (count < 0 && errno != EAGAIN) {
            PLOG(ERROR) << "Failed to read the slider input device";
        }

        int position;
        if (dropped && readPosition(&position)) {
            setPosition(position);
        }
    }
}

bool Slider::readPosition(int* position) {
    struct input_absinfo absinfo;
    if (ioctl(mFd, EVIOCGABS(mAbsCode), &absinfo) < 0) {
        PLOG(ERROR) << "Failed to read the slider axis";
        return false;
    }

    *position = absinfo.value - mMinimum;
    if (*position < 0 || *position >= mCount) {
        LOG(ERROR) << "Axis rests at " << absinfo.value << ", outside the range it reported";
        return false;
    }

    return true;
}

void Slider::setPosition(int position) {
    std::lock_guard<std::mutex> notifyLock(mNotifyMutex);
    if (mPosition.exchange(position) == position) {
        return;
    }

    notify(position);
}

void Slider::notify(int position) {
    std::vector<std::shared_ptr<ISliderCallback>> callbacks;
    {
        std::lock_guard<std::mutex> lock(mMutex);
        for (const auto& registration : mRegistrations) {
            callbacks.push_back(registration.callback);
        }
    }

    for (const auto& callback : callbacks) {
        ndk::ScopedAStatus status = callback->onPositionChanged(position);
        if (!status.isOk()) {
            LOG(ERROR) << "Failed to report position " << position << ", "
                       << status.getDescription();
        }
    }
}

ndk::ScopedAStatus Slider::getSliderInfo(SliderInfo* _aidl_return) {
    _aidl_return->positionCount = mCount;
    _aidl_return->location = mLocation;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Slider::getPosition(int32_t* _aidl_return) {
    *_aidl_return = mPosition.load();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Slider::registerCallback(const std::shared_ptr<ISliderCallback>& callback) {
    if (callback == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_NULL_POINTER);
    }

    // Held across the delivery below, so a client cannot be told a position
    // that a change racing this call has already superseded.
    std::lock_guard<std::mutex> notifyLock(mNotifyMutex);
    ndk::SpAIBinder binder = callback->asBinder();

    {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = std::find_if(mRegistrations.begin(), mRegistrations.end(),
                               [&](const Registration& r) { return r.binder == binder; });
        if (it == mRegistrations.end()) {
            // The cookie identifies this link for as long as it exists, which
            // a binder address does not once the client is released.
            // onCookieUnlinked frees this, and a failed link has already
            // run it.
            auto* cookie = new DeathCookie{this};
            binder_status_t status =
                    AIBinder_linkToDeath(binder.get(), mDeathRecipient.get(), cookie);
            if (status != STATUS_OK) {
                return ndk::ScopedAStatus::fromStatus(status);
            }

            mRegistrations.push_back({callback, binder, cookie});
        }
    }

    // The registration stays, since a client that cannot be reached is one the
    // death recipient is about to drop.
    ndk::ScopedAStatus status = callback->onPositionChanged(mPosition.load());
    if (!status.isOk()) {
        LOG(ERROR) << "Failed to report the current position, " << status.getDescription();
        return status;
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Slider::unregisterCallback(const std::shared_ptr<ISliderCallback>& callback) {
    if (callback == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_NULL_POINTER);
    }

    // A delivery in progress holds this, so a callback that has been
    // unregistered is one that is no longer being called.
    std::lock_guard<std::mutex> notifyLock(mNotifyMutex);
    std::lock_guard<std::mutex> lock(mMutex);
    ndk::SpAIBinder binder = callback->asBinder();

    auto it = std::find_if(mRegistrations.begin(), mRegistrations.end(),
                           [&](const Registration& r) { return r.binder == binder; });
    if (it == mRegistrations.end()) {
        return ndk::ScopedAStatus::ok();
    }

    binder_status_t status =
            AIBinder_unlinkToDeath(binder.get(), mDeathRecipient.get(), it->cookie);
    if (status != STATUS_OK) {
        LOG(WARNING) << "Failed to unlink a callback, status " << status;
    }

    mRegistrations.erase(it);
    return ndk::ScopedAStatus::ok();
}

void Slider::handleDeath(void* cookie) {
    std::lock_guard<std::mutex> lock(mMutex);
    auto it = std::find_if(mRegistrations.begin(), mRegistrations.end(),
                           [&](const Registration& r) { return r.cookie == cookie; });
    if (it == mRegistrations.end()) {
        return;
    }

    mRegistrations.erase(it);
}

void Slider::onCookieUnlinked(void* cookie) {
    delete static_cast<DeathCookie*>(cookie);
}

void Slider::onCallbackDied(void* cookie) {
    static_cast<DeathCookie*>(cookie)->slider->handleDeath(cookie);
}

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
