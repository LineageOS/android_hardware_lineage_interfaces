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
 * A Linux LED device.
 * @see https://docs.kernel.org/leds/leds-class.html
 */
class LedDevice : public IDevice {
  public:
    /**
     * LED light mode.
     */
    enum LightMode {
        /**
         * Static steady light.
         */
        STATIC,

        /**
         * Hardware specific breathing effect.
         */
        BREATH,

        /**
         * Upstream implementation of timed blinking.
         */
        TIMED_UPSTREAM,

        /**
         * Qualcomm implementation of timed blinking.
         */
        TIMED_QCOM,
    };

    LedDevice() = delete;

    /**
     * Constructor.
     *
     * @param name The name of the LED device
     */
    LedDevice(std::string name);

    bool isOk() const override;
    bool setState(const State& state) override;
    void dump(int fd) const override;

    /**
     * Get the name of the LED device.
     *
     * @return std::string The name of the LED device
     */
    std::string getName() const;

    /**
     * Return whether this LED device supports the given light mode.
     *
     * @param mode The light mode to check
     * @return bool true if the LED device supports the given mode, false otherwise
     */
    bool supportsMode(LightMode mode) const;

    /**
     * Set the brightness of the LED device.
     *
     * @param value The brightness value to set
     * @param mode The light mode to use
     * @return bool true if the brightness was set successfully, false otherwise
     */
    bool setBrightness(uint8_t value, LightMode mode = LightMode::STATIC, uint32_t flashOnMs = 0,
                       uint32_t flashOffMs = 0);

    /**
     * Set the index of the LED device.
     *
     * @param idx The index to set
     */
    void setIdx(int idx);

  private:
    std::string mName;
    int mIdx;
    std::string mBasePath;
    uint32_t mMaxBrightness;
    std::string mBreathNode;
    bool mSupportsTimedUpstream;
    bool mSupportsTimedQcom;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
