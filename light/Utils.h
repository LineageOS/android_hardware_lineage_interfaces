/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <models/State.h>

#include <aidl/android/hardware/light/HwLightState.h>
#include <cstdint>
#include <fstream>
#include <string>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

State fromAidl(const HwLightState& value);

uint32_t scaleBrightness(uint8_t brightness, uint32_t maxBrightness);

template <typename T>
bool readFromFile(const std::string& file, T& content) {
    std::ifstream fileStream(file);

    if (!fileStream) {
        return false;
    }

    fileStream >> content;
    return true;
}

template <typename T>
bool writeToFile(const std::string& file, const T content) {
    std::ofstream fileStream(file);

    if (!fileStream) {
        return false;
    }

    fileStream << content;
    return true;
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
