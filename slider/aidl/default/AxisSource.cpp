/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AxisSource.h"

#include <android-base/logging.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cerrno>

#include "InputDevice.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

namespace {
constexpr size_t kBitsPerWord = sizeof(unsigned long) * 8;
}  // namespace

AxisSource::AxisSource(const std::vector<std::string>& inputNames, int absCode)
    : mInputNames(inputNames), mAbsCode(absCode) {}

AxisSource::~AxisSource() {
    if (mFd >= 0) {
        close(mFd);
    }
}

bool AxisSource::open(bool verbose) {
    // A code outside the axis range would index absBits out of bounds.
    if (mAbsCode < 0 || mAbsCode > ABS_MAX) {
        LOG(ERROR) << "Axis code " << mAbsCode << " is not an absolute axis";
        return false;
    }

    mFd = openInputDevice(mInputNames, verbose, [&](int fd, const std::string& name) {
        unsigned long absBits[(ABS_CNT + kBitsPerWord - 1) / kBitsPerWord] = {};
        if (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absBits)), absBits) < 0 ||
            !(absBits[mAbsCode / kBitsPerWord] >> (mAbsCode % kBitsPerWord) & 1)) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " does not report abs code " << mAbsCode;
            }
            return false;
        }

        struct input_absinfo absinfo;
        if (ioctl(fd, EVIOCGABS(mAbsCode), &absinfo) < 0) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " has no abs code " << mAbsCode;
            }
            return false;
        }

        const int count = absinfo.maximum - absinfo.minimum + 1;
        if (count < 2) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " reports " << count << " slider positions";
            }
            return false;
        }

        const int position = absinfo.value - absinfo.minimum;
        if (position < 0 || position >= count) {
            if (verbose) {
                LOG(ERROR) << "Device " << name << " rests at " << absinfo.value << ", outside ["
                           << absinfo.minimum << ", " << absinfo.maximum << "]";
            }
            return false;
        }

        mCount = count;
        mMinimum = absinfo.minimum;
        return true;
    });

    return mFd >= 0;
}

std::optional<int> AxisSource::position() {
    struct input_absinfo absinfo;
    if (ioctl(mFd, EVIOCGABS(mAbsCode), &absinfo) < 0) {
        PLOG(ERROR) << "Failed to read the slider axis";
        return std::nullopt;
    }

    // An axis that does not begin at zero still reports positions from zero.
    const int position = absinfo.value - mMinimum;
    if (position < 0 || position >= mCount) {
        LOG(ERROR) << "Axis rests at " << absinfo.value << ", outside the range it reported";
        return std::nullopt;
    }

    return position;
}

void AxisSource::read(const std::function<void(int)>& moved) {
    // After SYN_DROPPED the queue describes a slider that has already moved
    // on, so it is drained and the axis is asked where it rests.
    bool dropped = false;
    struct input_event ev;
    ssize_t count;
    while ((count = TEMP_FAILURE_RETRY(::read(mFd, &ev, sizeof(ev)))) == sizeof(ev)) {
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

        moved(position);
    }

    if (count < 0 && errno != EAGAIN) {
        PLOG(ERROR) << "Failed to read the slider input device";
    }

    if (dropped) {
        if (std::optional<int> position = this->position(); position.has_value()) {
            moved(*position);
        }
    }
}

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
