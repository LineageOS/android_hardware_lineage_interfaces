/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <Devices.h>
#include <models/State.h>

#include <aidl/android/hardware/light/BnLights.h>
#include <mutex>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

class Lights : public BnLights {
  public:
    Lights();

    ndk::ScopedAStatus setLightState(int32_t id, const HwLightState& hwLightState) override;
    ndk::ScopedAStatus getLights(std::vector<HwLight>* _aidl_return) override;

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;

  private:
    std::vector<HwLight> mLights;

    Devices mDevices;

    State mLastBatteryState;
    State mLastNotificationsState;
    State mLastAttentionState;
    std::mutex mLedMutex;

    void updateNotificationColor();
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
