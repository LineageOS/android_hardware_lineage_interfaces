/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SwitchSource.h"

#include <android-base/logging.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>

#include "InputDevice.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

namespace {
constexpr size_t kSwitchWords = bitWords(SW_CNT);
}  // namespace

SwitchSource::SwitchSource(const std::vector<std::string>& inputNames,
                           const std::vector<int>& codes)
    : InputSource(inputNames),
      mCodes(codes),
      mCount(codes.size() == 1 ? 2 : static_cast<int>(codes.size())),
      mSet(codes.size(), false) {}

bool SwitchSource::open(bool verbose) {
    for (int code : mCodes) {
        // A code outside the switch range would index switchBits out of bounds.
        if (code < 0 || code >= SW_CNT) {
            LOG(ERROR) << "Switch code " << code << " is not a switch";
            return false;
        }
    }

    const bool opened = openDevice(verbose, [&](int fd, const std::string& name) {
        unsigned long switchBits[kSwitchWords] = {};
        if (ioctl(fd, EVIOCGBIT(EV_SW, sizeof(switchBits)), switchBits) < 0) {
            if (verbose) {
                PLOG(ERROR) << "Device " << name << " does not report its switch codes";
            }
            return false;
        }

        for (int code : mCodes) {
            if (!isBitSet(switchBits, code)) {
                if (verbose) {
                    LOG(ERROR) << "Device " << name << " does not report switch code " << code;
                }
                return false;
            }
        }

        return true;
    });

    if (!opened) {
        return false;
    }

    if (!position().has_value()) {
        if (verbose) {
            LOG(ERROR) << "The slider holds none of its switch codes set";
        }
        close(mFd);
        mFd = -1;
        return false;
    }

    return true;
}

// Queried rather than remembered, which EVIOCGSW makes destructive. It
// flushes every EV_SW event this client has queued, so it is called only
// where the state is taken wholesale afterwards.
// Each code sits at the position it names, and a lone code names both, set at
// the second and clear at the first. More than one reads set while the slider
// travels between detents, so the later position wins the tie.
std::optional<int> SwitchSource::positionOf(const std::vector<bool>& set) const {
    if (set.size() == 1) {
        return set[0] ? 1 : 0;
    }

    std::optional<int> position;
    for (size_t i = 0; i < set.size(); i++) {
        if (set[i]) {
            position = static_cast<int>(i);
        }
    }
    return position;
}

std::optional<int> SwitchSource::position() {
    unsigned long switchBits[kSwitchWords] = {};
    if (ioctl(mFd, EVIOCGSW(sizeof(switchBits)), switchBits) < 0) {
        PLOG(ERROR) << "Failed to read the slider switch state";
        return std::nullopt;
    }

    for (size_t i = 0; i < mCodes.size(); i++) {
        mSet[i] = isBitSet(switchBits, mCodes[i]);
    }

    return positionOf(mSet);
}

void SwitchSource::read(const std::function<void(int)>& moved) {
    // One move clears the code it left and sets the one it reached, so the
    // position is taken once the frame those two arrive in is complete.
    bool changed = false;
    const bool dropped = drain([&](const struct input_event& ev) {
        if (ev.type == EV_SW) {
            auto it = std::find(mCodes.begin(), mCodes.end(), ev.code);
            if (it != mCodes.end()) {
                mSet[std::distance(mCodes.begin(), it)] = ev.value != 0;
                changed = true;
            }
            return;
        }

        if (ev.type == EV_SYN && ev.code == SYN_REPORT && changed) {
            changed = false;
            if (std::optional<int> position = positionOf(mSet); position.has_value()) {
                moved(*position);
            }
        }
    });

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
