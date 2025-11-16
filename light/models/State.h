/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <models/Color.h>
#include <models/Effect.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/**
 * Device state.
 */
struct State {
    /**
     * The color.
     */
    Color color;

    /**
     * The effect.
     */
    Effect effect;

    /**
     * Return whether or not the light should be considered "on".
     */
    bool isLit();
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
