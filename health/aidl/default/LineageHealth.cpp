/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "LineageHealth.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android/binder_status.h>
#include <fstream>

#define LOG_TAG "vendor.lineage.health-service.default"

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

#ifndef CHARGING_ENABLED_PATH
#define CHARGING_ENABLED_PATH "/sys/class/power_supply/battery/charging_enabled"
#endif

ndk::ScopedAStatus LineageHealth::getChargingEnabled(bool* _aidl_return) {
    std::fstream chargingEnabledFile(CHARGING_ENABLED_PATH);

    if (!chargingEnabledFile.is_open()) {
        LOG(ERROR) << "Failed to open charging enabled node";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::string chargingEnabledResult;
    chargingEnabledFile >> chargingEnabledResult;

    if (chargingEnabledFile.fail()) {
        LOG(ERROR) << "Failed to read current charging enabled value";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (chargingEnabledResult == "0") {
        *_aidl_return = false;
    } else if (chargingEnabledResult == "1") {
        *_aidl_return = true;
    } else {
        LOG(ERROR) << "Unknown value " << chargingEnabledResult;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus LineageHealth::setChargingEnabled(bool enabled) {
    std::fstream chargingEnabledFile(CHARGING_ENABLED_PATH);

    if (!chargingEnabledFile.is_open()) {
        LOG(ERROR) << "Failed to open charging enabled node";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    chargingEnabledFile << (enabled ? "1" : "0");
    if (chargingEnabledFile.fail()) {
        LOG(ERROR) << "Failed to write to charging enable node: " << strerror(errno);
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
