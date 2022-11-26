/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ChargingControl.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android/binder_status.h>
#include <fstream>

#define LOG_TAG "vendor.lineage.health-service.default"

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

static const std::vector<ChargingEnabledNode> kChargingEnabledNodes = {
        {HEALTH_CHARGING_CONTROL_PATH, HEALTH_CHARGING_CONTROL_ENABLE,
         HEALTH_CHARGING_CONTROL_DISABLE},
        {"/sys/class/power_supply/battery/input_suspend", "0", "1"},
        {"/sys/class/power_supply/battery/charging_enabled", "1", "0"},
        {"/sys/class/qcom-battery/input_suspend", "0", "1"},
};

static bool fileExists(const std::string& path) {
    std::fstream fs(path);
    return fs.is_open();
}

ChargingControl::ChargingControl() : mChargingEnabledNode(nullptr) {
    for (const auto& node : kChargingEnabledNodes) {
        if (!fileExists(node.path)) {
            continue;
        }

        mChargingEnabledNode = &node;
        break;
    }

    if (!mChargingEnabledNode) {
        LOG(FATAL) << "Couldn't find a suitable charging control node";
    }
}

ndk::ScopedAStatus ChargingControl::getChargingEnabled(bool* _aidl_return) {
    std::fstream chargingEnabledFile(mChargingEnabledNode->path);

    if (!chargingEnabledFile.is_open()) {
        LOG(ERROR) << "Failed to open charging enabled node";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    std::string chargingEnabledResult;
    chargingEnabledFile >> chargingEnabledResult;

    if (chargingEnabledFile.fail()) {
        LOG(ERROR) << "Failed to read current charging enabled value";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (chargingEnabledResult == mChargingEnabledNode->value_true) {
        *_aidl_return = true;
    } else if (chargingEnabledResult == mChargingEnabledNode->value_false) {
        *_aidl_return = false;
    } else {
        LOG(ERROR) << "Unknown value " << chargingEnabledResult;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus ChargingControl::setChargingEnabled(bool enabled) {
    std::fstream chargingEnabledFile(mChargingEnabledNode->path);

    if (!chargingEnabledFile.is_open()) {
        LOG(ERROR) << "Failed to open charging enabled node";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    chargingEnabledFile << (enabled ? mChargingEnabledNode->value_true
                                    : mChargingEnabledNode->value_false);
    if (chargingEnabledFile.fail()) {
        LOG(ERROR) << "Failed to write to charging enable node: " << strerror(errno);
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

binder_status_t ChargingControl::dump(int fd, const char** /* args */, uint32_t /* numArgs */) {
    bool isChargingEnabled;
    getChargingEnabled(&isChargingEnabled);
    dprintf(fd, "Charging control node selected: %s\n", mChargingEnabledNode->path.c_str());
    dprintf(fd, "Charging enabled: %s\n", isChargingEnabled ? "true" : "false");

    return STATUS_OK;
}

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
