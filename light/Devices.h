/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifndef DISABLE_BACKLIGHT_CONTROL
#include <devices/BacklightDevice.h>
#endif
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

#ifndef DISABLE_BACKLIGHT_CONTROL
    bool hasBacklightDevices() const;
#endif
    bool hasButtonDevices() const;
    bool hasKeyboardDevices() const;
    bool hasNotificationDevices() const;

#ifndef DISABLE_BACKLIGHT_CONTROL
    void setBacklightState(const State& state);
#endif
    void setButtonsState(const State& state);
    void setKeyboardState(const State& state);
    void setNotificationState(const State& state);

  private:
    // Backlight
#ifndef DISABLE_BACKLIGHT_CONTROL
    std::vector<BacklightDevice> mBacklightDevices;
    std::vector<LedDevice> mBacklightLedDevices;
#endif

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
