/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <models/IDevice.h>

#include <cstdint>
#include <string>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/**
 * A Linux backlight device.
 * @see https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-class-backlight
 */
class BacklightDevice : public IDevice {
  public:
    BacklightDevice() = delete;

    /**
     * Constructor.
     *
     * @param name The name of the backlight device
     */
    BacklightDevice(std::string name);

    bool isOk() const override;
    bool setState(const State& state) override;
    void dump(int fd) const override;

    /**
     * Get the name of the backlight device.
     *
     * @return std::string The name of the backlight device
     */
    std::string getName() const;

    /**
     * Set the brightness of this backlight device.
     *
     * @param value The brightness value to set
     * @return bool true if the brightness was set successfully, false otherwise
     */
    bool setBrightness(uint8_t value);

  private:
    std::string mName;
    std::string mBasePath;
    uint32_t mMaxBrightness;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
