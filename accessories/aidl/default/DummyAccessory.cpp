/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "DummyAccessory.h"

#define LOG_TAG "DummyAccessory"

#include <android-base/logging.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

DummyAccessory::DummyAccessory(const AccessoryInfo& accessoryInfo,
                               const AccessoryStatus& accessoryStatus,
                               const BatteryInfo& batteryInfo) {
    mAccessoryInfo = accessoryInfo;
    mAccessoryStatus = accessoryStatus;
    mBatteryInfo = batteryInfo;
}

::ndk::ScopedAStatus DummyAccessory::setEnabled(bool enabled) {
    mAccessoryStatus = enabled ? AccessoryStatus::ENABLED : AccessoryStatus::DISABLED;

    onAccessoryStatusUpdated();

    return ::ndk::ScopedAStatus::ok();
}

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
