/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/accessories/Accessory.h>
#include <aidl/vendor/lineage/accessories/BnAccessory.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

/**
 * Dummy accessory.
 */
class DummyAccessory : public Accessory {
  public:
    DummyAccessory() = delete;
    DummyAccessory(const Accessory&) = delete;
    DummyAccessory(const AccessoryInfo& accessoryInfo, const AccessoryStatus& accessoryStatus,
                   const BatteryInfo& batteryInfo);

    ::ndk::ScopedAStatus setEnabled(bool enabled) override;
};

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
