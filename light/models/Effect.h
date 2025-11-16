/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/light/HwLightState.h>
#include <cstdint>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/**
 * Light effect.
 *
 * Each effect type has its own parameters.
 */
struct Effect {
    /**
     * Light effect type.
     */
    enum class Type {
        /**
         * Fixed color.
         */
        FIXED,

        /**
         * Timed blinking.
         */
        TIMED,

        /**
         * Hardware specific effect.
         */
        HARDWARE,
    };

    /**
     * The effect type. This determines which parameters are valid.
     *
     * @see Effect::Type
     */
    Type type = Type::FIXED;

    struct {
        uint32_t onMs;
        uint32_t offMs;
    } timed;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
