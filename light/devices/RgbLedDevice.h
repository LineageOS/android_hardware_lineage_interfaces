/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <devices/LedDevice.h>
#include <models/IDevice.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/**
 * A pool of LED devices that will be toggled based on the wanted color.
 * Support all types of LED combinations, with a maximum of 3 LEDs.
 * Also supports 2 color LEDs (*cough* ASUS *cough*).
 */
class RgbLedDevice : public IDevice {
  public:
    /**
     * LED roles.
     */
    enum Role {
        NONE = 0,
        RED = 1 << 0,
        GREEN = 1 << 1,
        BLUE = 1 << 2,
        ALL = RED | GREEN | BLUE,
    };

    RgbLedDevice() = delete;

    /**
     * Constructor.
     *
     * @param red The red LED device
     * @param green The green LED device
     * @param blue The blue LED device
     * @param rgbSyncNode The path to RGB sync trigger
     */
    RgbLedDevice(LedDevice red, LedDevice green, LedDevice blue, std::string rgbSyncNode);

    bool isOk() const override;
    bool setState(const State& state) override;
    void dump(int fd) const override;

    /**
     * Return whether this RGB LED device supports the given light mode.
     * This is true when all existing LEDs support the given mode.
     *
     * @return bool true if the RGB LED device supports the given mode, false otherwise
     */
    bool supportsMode(LedDevice::LightMode mode) const;

    /**
     * Return whether this RGB LED device supports RGB sync.
     *
     * @return bool true if the RGB LED device supports RGB sync, false otherwise
     */
    bool supportsRgbSync() const;

  private:
    LedDevice mRed;
    LedDevice mGreen;
    LedDevice mBlue;
    std::string mRgbSyncNode;

    int mRoles;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
