/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Slider.h"

#include <android-base/logging.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <utility>

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

namespace {
struct DeathCookie {
    Slider* slider;
};
}  // namespace

Slider::Slider(std::unique_ptr<Source> source, const std::optional<SliderLocation>& location)
    : mSource(std::move(source)), mLocation(location) {
    mDeathRecipient =
            ndk::ScopedAIBinder_DeathRecipient(AIBinder_DeathRecipient_new(&onCallbackDied));
    AIBinder_DeathRecipient_setOnUnlinked(mDeathRecipient.get(), &onCookieUnlinked);
}

bool Slider::start(bool verbose) {
    if (!mSource->open(verbose)) {
        return false;
    }

    std::optional<int> position = mSource->position();
    if (!position.has_value()) {
        return false;
    }

    mCount = mSource->count();
    mPosition.store(*position);
    LOG(INFO) << "Slider has " << mCount << " positions, at " << *position;

    return startEventLoop();
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

    const int descriptor = mSource->descriptor();
    struct epoll_event inputEvent = {.events = EPOLLIN, .data = {.fd = descriptor}};
    struct epoll_event stopEvent = {.events = EPOLLIN, .data = {.fd = mStopFd}};
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, descriptor, &inputEvent) < 0 ||
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

        mSource->read([this](int position) { setPosition(position); });
    }
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
