/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/health/BnCharger.h>
#include <aidl/vendor/lineage/health/ChargerPauseType.h>
#include <android-base/file.h>
#include "android/binder_status.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

#define SET_CHG_PAUSE_REASON(reason) (chgStopReason |= static_cast<int32_t>(x))

struct Charger : public BnCharger {
    Charger();
    ndk::ScopedAStatus setChargingPauseUser(bool enabled) override;
    ndk::ScopedAStatus getChargingPauseReason(int32_t* _aidl_return) override;
    ndk::ScopedAStatus setChargingLimit(int32_t limit) override;
    ndk::ScopedAStatus checkBatCapacityAndApplyLimit() override;

  private:
    bool updateChargingStatus();
    bool setChargingPauseInternal(bool enabled);

    /**
     * Clear or set the reason in setChgPauseReason
     * If the parameter |set| is true, then the reason will be set
     * otherwise this reason will be cleared
     */
    void setChgPauseReason(ChargerPauseType reason, bool set);

    int chgLimit;
    int chgStopReason;
};

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
