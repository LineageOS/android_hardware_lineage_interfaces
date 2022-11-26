/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Charger.h"

#include <aidl/vendor/lineage/health/ChargerPauseType.h>
#include <android-base/logging.h>
#include <android/binder_status.h>
#include <cutils/properties.h>
#include <cutils/uevent.h>
#include <string.h>
#include <fstream>

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

#define LOG_TAG "vendor.lineage.health-service.example"

static constexpr const char* kChgInterruptionPath =
        "/sys/class/power_supply/battery_ext/smart_charging_interruption";

static constexpr const char* kBatCapacityPath = "/sys/class/power_supply/battery/capacity";

Charger::Charger() {
    LOG(INFO) << "Charger HAL Starting...";
    chgLimit = 100;
    chgStopReason = 0;
}

void Charger::setChgPauseReason(ChargerPauseType reason, bool set) {
    if (set)
        chgStopReason |= static_cast<int32_t>(reason);
    else
        chgStopReason &= ~static_cast<int32_t>(reason);
}

bool Charger::updateChargingStatus() {
    // If chgStopReason is not 0, we need to stop charging
    bool enable = chgStopReason != 0;

    return setChargingPauseInternal(enable);
}

bool Charger::setChargingPauseInternal(bool enabled) {
    LOG(INFO) << (enabled ? "Enable" : "Disable") << " charging";
    std::fstream interruptionFile(kChgInterruptionPath);
    int interruptionResult = -1;

    interruptionFile >> interruptionResult;

    if (interruptionFile.fail()) goto error;

    if (interruptionResult != enabled) {
        interruptionFile << (enabled ? "1" : "0");
        if (interruptionFile.fail()) goto error;
    }

    return true;

error:
    LOG(ERROR) << "Failed to read or write charging status: " << strerror(errno);
    return false;
}

ndk::ScopedAStatus Charger::getChargingPauseReasion(int32_t* _aidl_return) {
    *_aidl_return = chgStopReason;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Charger::setChargingPauseUser(bool enabled) {
    if (enabled)
        setChgPauseReason(ChargerPauseType::CHARGER_PAUSE_FORCE, true);
    else
        setChgPauseReason(ChargerPauseType::CHARGER_PAUSE_FORCE, false);

    if (updateChargingStatus())
        return ndk::ScopedAStatus::ok();
    else
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
}

ndk::ScopedAStatus Charger::setChargingLimit(int32_t limit) {
    LOG(INFO) << "Setting charging limit to " << limit;

    chgLimit = limit;

    return checkBatCapacityAndApplyLimit();
}

ndk::ScopedAStatus Charger::checkBatCapacityAndApplyLimit() {
    std::ifstream batCapFile(kBatCapacityPath);
    int batCapNow = -1;

    batCapFile >> batCapNow;
    LOG(INFO) << "Capacity: " << batCapNow << ". Limit: " << chgLimit;

    if (chgLimit == 100) {
        setChgPauseReason(ChargerPauseType::CHARGER_PAUSE_LIMIT, false);
    } else {
        if (!batCapFile.is_open()) goto error;

        if (batCapNow >= chgLimit) {
            // Set limit
            setChgPauseReason(ChargerPauseType::CHARGER_PAUSE_LIMIT, true);
        } else {
            setChgPauseReason(ChargerPauseType::CHARGER_PAUSE_LIMIT, false);
        }
    }

    if (updateChargingStatus())
        return ndk::ScopedAStatus::ok();
    else
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);

error:
    LOG(ERROR) << "Failed to read charging status: " << strerror(errno);
    return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
}

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
