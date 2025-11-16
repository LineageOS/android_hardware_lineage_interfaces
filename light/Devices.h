/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <devices/BacklightDevice.h>
#include <devices/LedDevice.h>
#include <devices/RgbLedDevice.h>
#include <models/IDumpable.h>
#include <models/State.h>

#include <vector>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

class Devices : public IDumpable {
  public:
    Devices();

    void dump(int fd) const override;

    bool hasBacklightDevices() const;
    bool hasButtonDevices() const;
    bool hasKeyboardDevices() const;
    bool hasNotificationDevices() const;

    void setBacklightState(const State& state);
    void setButtonsState(const State& state);
    void setKeyboardState(const State& state);
    void setNotificationState(const State& state);

  private:
    // Backlight
    std::vector<BacklightDevice> mBacklightDevices;
    std::vector<LedDevice> mBacklightLedDevices;

    // Buttons
    std::vector<LedDevice> mButtonLedDevices;

    // Keyboard
    std::vector<LedDevice> mKeyboardLedDevices;

    // Notifications
    std::vector<RgbLedDevice> mNotificationRgbLedDevices;
    std::vector<LedDevice> mNotificationLedDevices;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
