/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/vendor/lineage/accessories/Accessory.h>

#define LOG_TAG "Accessory"

#include <android-base/logging.h>

using std::optional;
using std::shared_ptr;

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

::ndk::ScopedAStatus Accessory::getAccessoryInfo(AccessoryInfo* _aidl_return) {
    *_aidl_return = mAccessoryInfo;

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Accessory::getAccessoryStatus(AccessoryStatus* _aidl_return) {
    *_aidl_return = mAccessoryStatus;

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Accessory::setCallback(const shared_ptr<IAccessoryCallback>& callback) {
    mCallback = callback;

    // Notify the callback of all the accessory info
    onAccessoryInfoUpdated();
    onAccessoryStatusUpdated();
    onBatteryInfoUpdated();

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Accessory::getBatteryInfo(BatteryInfo* _aidl_return) {
    if (!mAccessoryInfo.supportsBatteryQuerying) {
        // Battery info must not exist if the accessory does not support it
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (mAccessoryStatus == AccessoryStatus::DISCONNECTED) {
        // Battery info must not exist if the accessory is disconnected
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = mBatteryInfo;

    return ::ndk::ScopedAStatus::ok();
}

binder_status_t Accessory::dump(int fd, const char** /*args*/, uint32_t /*numArgs*/) {
    ::ndk::ScopedAStatus ret;

    AccessoryInfo accessoryInfo;
    AccessoryStatus accessoryStatus;
    BatteryInfo batteryInfo;
    bool hasBatteryInfo = false;

    // Get info
    getAccessoryInfo(&accessoryInfo);
    getAccessoryStatus(&accessoryStatus);
    if (getBatteryInfo(&batteryInfo).isOk()) {
        hasBatteryInfo = true;
    }

    dprintf(fd, "Accessory:\n");
    dprintf(fd, "  id: %s\n", accessoryInfo.id.c_str());
    dprintf(fd, "  type: %d\n", static_cast<int>(accessoryInfo.type));
    dprintf(fd, "  isPersistent: %d\n", accessoryInfo.isPersistent);
    dprintf(fd, "  displayName: %s\n", accessoryInfo.displayName.c_str());
    dprintf(fd, "  displayId: %s\n", accessoryInfo.displayId.c_str());
    dprintf(fd, "  supportsToggling: %d\n", accessoryInfo.supportsToggling);
    dprintf(fd, "  supportsBatteryQuerying: %d\n", accessoryInfo.supportsBatteryQuerying);
    dprintf(fd, "  status: %d\n", static_cast<int>(accessoryStatus));
    if (hasBatteryInfo) {
        dprintf(fd, "  battery status: %d\n", static_cast<int>(batteryInfo.status));
        dprintf(fd, "  battery level: %d\n", batteryInfo.levelPercentage);
    }

    return STATUS_OK;
}

void Accessory::onAccessoryInfoUpdated() {
    if (mCallback != nullptr) {
        mCallback->onAccessoryInfoUpdated(mAccessoryInfo);
    }
}

void Accessory::onAccessoryStatusUpdated() {
    if (mCallback != nullptr) {
        mCallback->onAccessoryStatusUpdated(mAccessoryStatus);
    }

    // Also update battery info on accessory status change
    onBatteryInfoUpdated();
}

void Accessory::onBatteryInfoUpdated() {
    // Do not update battery info if the accessory does not support it
    if (!mAccessoryInfo.supportsBatteryQuerying) {
        return;
    }

    optional<BatteryInfo> batteryInfo = mBatteryInfo;
    if (mAccessoryStatus == AccessoryStatus::DISCONNECTED) {
        // Battery info must not exist if the accessory is disconnected
        batteryInfo.reset();
    }

    if (mCallback != nullptr) {
        mCallback->onBatteryInfoUpdated(batteryInfo);
    }
}

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
