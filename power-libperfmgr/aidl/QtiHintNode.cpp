/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define ATRACE_TAG (ATRACE_TAG_POWER | ATRACE_TAG_HAL)
#define LOG_TAG "libperfmgr"

#include "QtiHintNode.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>

#include <algorithm>

#include "HintManagerQti.h"

using namespace std::chrono_literals;

namespace aidl {
namespace lineage {
namespace hardware {
namespace power {
namespace impl {
namespace qti {

using ::android::perfmgr::RequestGroup;

namespace {
constexpr auto kRetryUpdatePeriod = 500ms;
constexpr auto kVendorDefaultTimeout = -1ms;
constexpr int kInvalidHandle = -1;
constexpr char kPerfPackage[] = "QTI PowerHAL";

bool CheckHandle(int handle) {
    return handle > 0;
}

}  // anonymous namespace

QtiHintNode::QtiHintNode(std::string name, int hint_id, int hint_type, const PerfdBinding &binding,
                         std::optional<int> inactive_hint_id)
    : Node(std::move(name), {},
           {RequestGroup("timed"), RequestGroup("no_timeout"), RequestGroup("vendor_default"),
            RequestGroup("inactive")},
           kInactiveActionValueIndex, false),
      hint_id_(hint_id),
      hint_type_(hint_type),
      inactive_hint_id_(inactive_hint_id),
      binding_(binding) {}

::android::perfmgr::NodeAction QtiHintNode::GetNodeAction(std::size_t node_index,
                                                          std::chrono::milliseconds timeout_ms) {
    // Vendor default timeout asks the backend to choose the actual duration.
    // Frontend cannot observe its expiry, so keep it separate from no-timeout requests.
    if (timeout_ms == kVendorDefaultTimeout) {
        return {node_index, kVendorDefaultActionValueIndex, std::chrono::milliseconds::zero()};
    }
    if (timeout_ms == std::chrono::milliseconds::zero()) {
        return {node_index, kNoTimeoutActionValueIndex, std::chrono::milliseconds::zero()};
    }
    return {node_index, kTimedActionValueIndex, timeout_ms};
}

std::chrono::milliseconds QtiHintNode::Update(bool log_error) {
    std::size_t value_index = kInactiveActionValueIndex;
    std::chrono::milliseconds expire_time = std::chrono::milliseconds::max();

    // Find the highest outstanding request's expire time.
    for (std::size_t i = 0; i < kInactiveActionValueIndex; i++) {
        if (req_sorted_[i].GetExpireTime(&expire_time)) {
            value_index = i;
            break;
        }
    }

    if (value_index != current_val_index_ || reset_on_init_) {
        bool successfully_updated = true;
        if (value_index == kInactiveActionValueIndex) {
            if (current_val_index_ != kInactiveActionValueIndex) {
                if (inactive_hint_id_.has_value()) {
                    CallPerfHint(*inactive_hint_id_, 0, false);
                } else {
                    ReleaseHandle(log_error);
                }
            }
        } else {
            successfully_updated = ApplyActiveHint(value_index, log_error);
        }

        if (successfully_updated) {
            current_val_index_ = value_index;
            reset_on_init_ = false;
        } else {
            expire_time = std::min(expire_time, kRetryUpdatePeriod);
        }
    }

    return expire_time;
}

void QtiHintNode::DumpToFd(int fd) const {
    const char *state = "inactive";
    if (current_val_index_ == kTimedActionValueIndex) {
        state = inactive_hint_id_.has_value() ? "active" : "timed";
    } else if (current_val_index_ == kNoTimeoutActionValueIndex) {
        state = inactive_hint_id_.has_value() ? "active" : "no_timeout";
    } else if (current_val_index_ == kVendorDefaultActionValueIndex) {
        state = inactive_hint_id_.has_value() ? "active" : "vendor_default";
    }

    std::string buf;
    if (inactive_hint_id_.has_value()) {
        buf = android::base::StringPrintf(
                "%s\tstate=%s\tactive_hint_id=%d\tinactive_hint_id=%d\ttype=%d\n", name_.c_str(),
                state, hint_id_, *inactive_hint_id_, hint_type_);
    } else {
        buf = android::base::StringPrintf("%s\tstate=%s\thandle=%d\thint_id=%d\ttype=%d\n",
                                          name_.c_str(), state, handle_, hint_id_, hint_type_);
    }
    if (!android::base::WriteStringToFd(buf, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }

    for (std::size_t i = 0; i < req_sorted_.size(); i++) {
        req_sorted_[i].DumpToFd(fd, android::base::StringPrintf("\t\tReq%zu:\t", i));
    }
}

bool QtiHintNode::ApplyActiveHint(std::size_t value_index, bool log_error) {
    if (inactive_hint_id_.has_value()) {
        return CheckHandle(CallPerfHint(hint_id_, 0, log_error));
    }

    const int hal_duration_ms = value_index == kVendorDefaultActionValueIndex ? -1 : 0;
    int ret = -1;

    if (binding_.perfHintAcqRel != nullptr) {
        ret = binding_.perfHintAcqRel(CheckHandle(handle_) ? handle_ : kInvalidHandle, hint_id_,
                                      kPerfPackage, hal_duration_ms, hint_type_, 0, nullptr);
    } else {
        if (CheckHandle(handle_) && binding_.releasePerfLock(handle_) < 0 && log_error) {
            LOG(ERROR) << "Failed to release QTI perf handle for node " << name_
                       << " handle=" << handle_;
        }
        ret = CallPerfHint(hint_id_, hal_duration_ms, log_error);
    }

    if (!CheckHandle(ret)) {
        if (binding_.perfHintAcqRel != nullptr && log_error) {
            LOG(ERROR) << "Failed to issue QTI perf action id=" << hint_id_
                       << " type=" << hint_type_ << " duration_ms=" << hal_duration_ms;
        }
        handle_ = kInvalidHandle;
        return false;
    }

    handle_ = ret;
    return true;
}

int QtiHintNode::CallPerfHint(int hint_id, int hal_duration_ms, bool log_error) {
    const int ret = binding_.perfHint(hint_id, kPerfPackage, hal_duration_ms, hint_type_);
    if (!CheckHandle(ret)) {
        if (log_error) {
            LOG(ERROR) << "Failed to issue QTI perf action id=" << hint_id << " type=" << hint_type_
                       << " duration_ms=" << hal_duration_ms;
        }
        return kInvalidHandle;
    }

    return ret;
}

void QtiHintNode::ReleaseHandle(bool log_error) {
    if (!CheckHandle(handle_)) {
        handle_ = kInvalidHandle;
        return;
    }

    if (binding_.releasePerfLock(handle_) < 0 && log_error) {
        LOG(ERROR) << "Failed to release QTI perf handle for node " << name_
                   << " handle=" << handle_;
    }
    handle_ = kInvalidHandle;
}

}  // namespace qti
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace lineage
}  // namespace aidl
