/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Utils.h>

#define LOG_TAG "Utils"

#include <aidl/android/hardware/light/FlashMode.h>
#include <android-base/logging.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

State fromAidl(const HwLightState& value) {
    Effect effect;

    switch (value.flashMode) {
        case FlashMode::NONE:
            effect.type = Effect::Type::FIXED;
            break;
        case FlashMode::TIMED:
            effect.type = Effect::Type::TIMED;
            effect.timed.onMs = value.flashOnMs;
            effect.timed.offMs = value.flashOffMs;
            break;
        case FlashMode::HARDWARE:
            effect.type = Effect::Type::HARDWARE;
            break;
        default:
            LOG(ERROR) << "Unknown flash mode: " << static_cast<int>(value.flashMode);
            effect.type = Effect::Type::FIXED;
            break;
    }

    return State{
            .color = Color(value.color),
            .effect = effect,
    };
}

uint32_t scaleBrightness(uint8_t brightness, uint32_t maxBrightness) {
    return brightness * maxBrightness / 0xFF;
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
