/*
 * Copyright (C) 2017 The LineageOS Project
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

#include "Vibrator.h"

#include <cmath>
#include <fstream>

namespace vendor {
namespace lineage {
namespace vibrator {
namespace V1_0 {
namespace implementation {

#define VIBRATOR "/sys/class/timed_output/vibrator/"

#define ENABLE      "enable"
#define VTG_DEFAULT "vtg_default"
#define VTG_LEVEL   "vtg_level"
#define VTG_MIN     "vtg_min"
#define VTG_MAX     "vtg_max"

#define CLICK_TIMING_MS 20

#define DEFAULT_MIN_VTG 0
#define DEFAULT_MAX_VTG 255

static int get(std::string path, int defaultValue) {
    int value;
    std::ifstream file(path);

    if (!file) {
        ALOGE("Failed to open %s", path.c_str());
        return defaultValue;
    }

    file >> value;

    return value;
}

static int set(std::string path, int value) {
    std::ofstream file(path);

    if (!file) {
        ALOGE("Failed to open %s", path.c_str());
        return -1;
    }

    file << value;

    return 0;
}

Vibrator::Vibrator() {
    defaultVoltage = get(VIBRATOR VTG_DEFAULT, DEFAULT_MAX_VTG);
    minVoltage = get(VIBRATOR VTG_MIN, DEFAULT_MIN_VTG);
    maxVoltage = get(VIBRATOR VTG_MAX, DEFAULT_MAX_VTG);
}

// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
Return<Status> Vibrator::on(uint32_t timeoutMs) {
    if (set(VIBRATOR ENABLE, timeoutMs)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

Return<Status> Vibrator::off() {
    if (set(VIBRATOR ENABLE, 0)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl() {
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

    if (set(VIBRATOR VTG_LEVEL, voltage)) {
        return Status::UNKNOWN_ERROR;
    }

    ALOGI("Voltage set to: %u", voltage);

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

// Methods from ::vendor::lineage::vibrator::V1_0::IVibrator follow.

Return<void> Vibrator::getDefaultAmplitude(getDefaultAmplitude_cb _hidl_cb) {
    /*
     * Scale the default voltage to be within 1 and 255 inclusive.
     */
    uint8_t defaultAmplitude =
            std::lround((254 * (defaultVoltage - minVoltage)) / (maxVoltage - minVoltage) + 1);

    _hidl_cb(Status::OK, defaultAmplitude);

    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor
