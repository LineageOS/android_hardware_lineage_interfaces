/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android-base/macros.h>
#include <perfmgr/Node.h>
#include <perfmgr/NodeLooperThread.h>

#include <optional>

namespace aidl {
namespace lineage {
namespace hardware {
namespace power {
namespace impl {
namespace qti {

class PerfdBinding;

class QtiHintNode : public ::android::perfmgr::Node {
  public:
    QtiHintNode(std::string name, int hint_id, int hint_type, const PerfdBinding &binding,
                std::optional<int> inactive_hint_id = std::nullopt);

    static ::android::perfmgr::NodeAction GetNodeAction(std::size_t node_index,
                                                        std::chrono::milliseconds timeout_ms);

    std::chrono::milliseconds Update(bool log_error) override;
    void DumpToFd(int fd) const override;

  private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(QtiHintNode);

    bool ApplyActiveHint(std::size_t value_index, bool log_error);
    int CallPerfHint(int hint_id, int hal_duration_ms, bool log_error);
    void ReleaseHandle(bool log_error);

    enum ValueIndex {
        kTimedActionValueIndex,
        kNoTimeoutActionValueIndex,
        kVendorDefaultActionValueIndex,
        kInactiveActionValueIndex,
    };

    const int hint_id_;
    const int hint_type_;
    const std::optional<int> inactive_hint_id_;
    const PerfdBinding &binding_;
    int handle_ = -1;
};

}  // namespace qti
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace lineage
}  // namespace aidl
