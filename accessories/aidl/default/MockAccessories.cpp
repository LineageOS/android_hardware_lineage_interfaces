/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "MockAccessories.h"

#define LOG_TAG "MockAccessories"

#include <android-base/logging.h>
#include "DummyAccessory.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

MockAccessories::MockAccessories() {
    addAccessory(::ndk::SharedRefBase::make<DummyAccessory>(
            (AccessoryInfo){
                    .id = "keyboard:dummy:1",
                    .type = AccessoryType::KEYBOARD,
                    .isPersistent = true,
                    .displayName = "Dummy Keyboard",
                    .displayId = "123456",
                    .supportsToggling = true,
                    .supportsBatteryQuerying = true,
            },
            AccessoryStatus::ENABLED,
            (BatteryInfo){
                    .status = BatteryStatus::NOT_CHARGING,
                    .levelPercentage = 100,
            }));

    addAccessory(::ndk::SharedRefBase::make<DummyAccessory>(
            (AccessoryInfo){
                    .id = "pen:dummy:1",
                    .type = AccessoryType::ACTIVE_PEN,
                    .isPersistent = true,
                    .displayName = "Dummy Active Pen",
                    .displayId = "123456",
                    .supportsToggling = true,
                    .supportsBatteryQuerying = true,
            },
            AccessoryStatus::ENABLED,
            (BatteryInfo){
                    .status = BatteryStatus::NOT_CHARGING,
                    .levelPercentage = 100,
            }));
}

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
