/*
 * Copyright (C) 2025 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "powerhal-libperfmgr"
#include "TaskRampupMultNode.h"

#include <android-base/file.h>
#include <android-base/logging.h>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

static constexpr char taskRampupSetPath[] = "/proc/vendor_sched/sched_qos/rampup_multiplier_set";

TaskRampupMultNode::TaskRampupMultNode() {
    if (access(taskRampupSetPath, W_OK) != 0) {
        mTaskRampupFd = -1;
        LOG(WARNING) << "Can't find vendor node: " << taskRampupSetPath;
        return;
    }

    int flags = O_WRONLY | O_TRUNC | O_CLOEXEC;
    mTaskRampupFd = TEMP_FAILURE_RETRY(open(taskRampupSetPath, flags));
    if (mTaskRampupFd < 0) {
        LOG(ERROR) << "Failed to open the node: " << taskRampupSetPath;
    }
}

TaskRampupMultNode::~TaskRampupMultNode() {
    if (mTaskRampupFd >= 0) {
        ::close(mTaskRampupFd);
    }
}

bool TaskRampupMultNode::updateTaskRampupMult(int32_t tid, int32_t val) {
    std::lock_guard lock(mMutex);
    if (mTaskRampupFd < 0) {
        LOG(WARNING) << "Invalid task tampup multiplier file descriptor, skipping the update";
        return false;
    }

    // Prepare the tid and value pair in the required format for the vendor procfs node.
    std::string tidValPair = std::to_string(tid) + ":" + std::to_string(val);
    if (!::android::base::WriteStringToFd(tidValPair, mTaskRampupFd)) {
        LOG(ERROR) << "Failed to write the new value " << tidValPair
                   << " to task rampup multiplier node.";
        return false;
    }
    return true;
}

bool TaskRampupMultNode::isValid() const {
    return mTaskRampupFd >= 0;
}

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
