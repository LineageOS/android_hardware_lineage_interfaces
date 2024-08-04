/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/accessories/BnAccessory.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

/**
 * Base accessory class with helper methods.
 */
class Accessory : public BnAccessory {
  public:
    Accessory() = default;
    Accessory(const Accessory&) = delete;

    ::ndk::ScopedAStatus getAccessoryInfo(AccessoryInfo* _aidl_return) override;
    ::ndk::ScopedAStatus getAccessoryStatus(AccessoryStatus* _aidl_return) override;
    ::ndk::ScopedAStatus setCallback(
            const ::std::shared_ptr<IAccessoryCallback>& callback) override;
    ::ndk::ScopedAStatus getBatteryInfo(BatteryInfo* _aidl_return) override;

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;

  protected:
    /**
     * Call this when mAccessoryInfo has been updated.
     */
    void onAccessoryInfoUpdated();

    /**
     * Call this when mAccessoryStatus has been updated.
     */
    void onAccessoryStatusUpdated();

    /**
     * Call this when mBatteryInfo has been updated.
     */
    void onBatteryInfoUpdated();

    /**
     * The cached accessory informations, this will be returned by getAccessoryInfo().
     * This should be updated by the subclass and subsequently call onAccessoryInfoUpdated().
     */
    AccessoryInfo mAccessoryInfo;

    /**
     * The cached accessory status, this will be returned by getAccessoryStatus().
     * This should be updated by the subclass and subsequently call onAccessoryStatusUpdated().
     */
    AccessoryStatus mAccessoryStatus;

    /**
     * The cached battery informations, this will be returned by getBatteryInfo().
     * This should be updated by the subclass and subsequently call onBatteryInfoUpdated().
     */
    BatteryInfo mBatteryInfo;

  private:
    ::std::shared_ptr<IAccessoryCallback> mCallback;
};

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
