/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "VibratorService"

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/vibrator.h>

#include "Vibrator.h"

#include <cinttypes>
#include <cmath>
#include <iostream>
#include <fstream>

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_0 {
namespace implementation {

#define VIBRATOR "/sys/class/timed_output/vibrator/"

#define ENABLE      "enable"
#define VTG_LEVEL   "vtg_level"
#define VTG_MIN     "vtg_min"
#define VTG_MAX     "vtg_max"

static int get(std::string path) {
    int value;
    std::ifstream file(path);
    file >> value;
    return value;
}

static void set(std::string path, int value) {
    std::ofstream file(path);
    file << value;
}

Vibrator::Vibrator() {
    minVoltage = get(VIBRATOR VTG_MIN);
    maxVoltage = get(VIBRATOR VTG_MAX);
}

Return<Status> Vibrator::on(uint32_t timeout_ms) {
    set(VIBRATOR ENABLE, timeout_ms);

    return Status::OK;
}

Return<Status> Vibrator::off()  {
    set(VIBRATOR ENABLE, 0);

    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl()  {
    return true;
}

Return<Status> Vibrator::setAmplitude(uint8_t amplitude) {
    if (amplitude == 0) {
        return Status::BAD_VALUE;
    }

    /*
     * Scale the voltage such that an amplitude of 1 is minVoltage
     * and an amplitude of 255 is maxVoltage.
     */
    uint32_t voltage =
            std::lround((amplitude - 1) / 254.0 * (maxVoltage - minVoltage) + minVoltage);
    set(VIBRATOR VTG_LEVEL, voltage);
    ALOGI("voltage: %ld", voltage);

    return Status::OK;
}

Return<void> Vibrator::perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    switch (effect) {
    case Effect::CLICK:
        uint8_t amplitude;

        switch (strength) {
        case EffectStrength::LIGHT:
            amplitude = 36;
            break;
        case EffectStrength::MEDIUM:
            amplitude = 128;
            break;
        case EffectStrength::STRONG:
            amplitude = 255;
            break;
        default:
            _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
            return Void();
        }

        setAmplitude(amplitude);
        on(CLICK_TIMING_MS);
        _hidl_cb(Status::OK, CLICK_TIMING_MS);
        break;
    default:
        _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
        break;
    }

    return Void();
}

} // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace hardware
}  // namespace android
