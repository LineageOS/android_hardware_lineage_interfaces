/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "InputSource.h"

#include <android-base/logging.h>
#include <unistd.h>

#include <cerrno>

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

InputSource::InputSource(const std::vector<std::string>& inputNames) : mInputNames(inputNames) {}

InputSource::~InputSource() {
    if (mFd >= 0) {
        close(mFd);
    }
}

bool InputSource::drain(const std::function<void(const struct input_event&)>& handle) {
    bool dropped = false;
    struct input_event ev;
    ssize_t count;
    while ((count = TEMP_FAILURE_RETRY(::read(mFd, &ev, sizeof(ev)))) == sizeof(ev)) {
        if (ev.type == EV_SYN && ev.code == SYN_DROPPED) {
            LOG(WARNING) << "Dropped slider events, reading the hardware instead";
            dropped = true;
            continue;
        }

        if (!dropped) {
            handle(ev);
        }
    }

    if (count < 0 && errno != EAGAIN) {
        PLOG(ERROR) << "Failed to read the slider input device";
    }

    return dropped;
}

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
