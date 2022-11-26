/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/health/BnLineageHealth.h>
#include "android/binder_status.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

struct ChargingEnabledNode {
    const std::string path;
    const std::string value_true;
    const std::string value_false;
};

struct LineageHealth : public BnLineageHealth {
    LineageHealth::LineageHealth();

    ndk::ScopedAStatus getChargingEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setChargingEnabled(bool enabled) override;

  private:
    ChargingEnabledNode* mChargingEnabledNode;
};

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
