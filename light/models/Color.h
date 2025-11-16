/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/**
 * 32-bit representation of a color.
 */
struct Color {
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    Color(uint32_t color);

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    bool isLit();
    uint8_t toBrightness();
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
