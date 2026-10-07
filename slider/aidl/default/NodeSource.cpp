/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "NodeSource.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <linux/input.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>

#include "InputDevice.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

NodeSource::NodeSource(const std::vector<std::string>& inputNames, const std::string& node,
                       const std::vector<int>& values)
    : mInputNames(inputNames), mNode(node), mValues(values) {}

NodeSource::~NodeSource() {
    if (mFd >= 0) {
        close(mFd);
    }
}

bool NodeSource::open(bool verbose) {
    if (!position().has_value()) {
        if (verbose) {
            LOG(ERROR) << mNode << " does not hold a position this slider has";
        }
        return false;
    }

    // Any event from the device means the node may have changed, so nothing
    // about the device itself has to be accepted beyond its name.
    mFd = openInputDevice(mInputNames, verbose, [](int, const std::string&) { return true; });

    return mFd >= 0;
}

std::optional<int> NodeSource::position() {
    std::string content;
    if (!::android::base::ReadFileToString(mNode, &content)) {
        PLOG(ERROR) << "Failed to read " << mNode;
        return std::nullopt;
    }

    int value;
    if (!::android::base::ParseInt(::android::base::Trim(content), &value)) {
        LOG(ERROR) << mNode << " holds " << ::android::base::Trim(content) << ", not a value";
        return std::nullopt;
    }

    auto it = std::find(mValues.begin(), mValues.end(), value);
    if (it == mValues.end()) {
        LOG(ERROR) << mNode << " holds " << value << ", which is no position of this slider";
        return std::nullopt;
    }

    return static_cast<int>(std::distance(mValues.begin(), it));
}

void NodeSource::read(const std::function<void(int)>& moved) {
    // The events carry no position, so they are drained and the node is asked
    // where the slider is now.
    struct input_event ev;
    ssize_t count;
    while ((count = TEMP_FAILURE_RETRY(::read(mFd, &ev, sizeof(ev)))) == sizeof(ev)) {
    }

    if (count < 0 && errno != EAGAIN) {
        PLOG(ERROR) << "Failed to read the slider input device";
    }

    if (std::optional<int> position = this->position(); position.has_value()) {
        moved(*position);
    }
}

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
