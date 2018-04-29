/*
 * Copyright (C) 2017-2018 The LineageOS Project
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

#define VIBRATOR "/sys/devices/virtual/timed_output/vibrator/"

#define ENABLE      "enable"

#define VTG_DEFAULT "vtg_default"
#define VTG_LEVEL   "vtg_level"
#define VTG_MIN     "vtg_min"
#define VTG_MAX     "vtg_max"

#define VTG_LIGHT   "vtg_light"
#define VTG_MEDIUM  "vtg_medium"
#define VTG_STRONG  "vtg_strong"

#define CLICK_TIMING_MS 20

#define DEFAULT_MIN_VTG 0
#define DEFAULT_MAX_VTG 255
#define DEFAULT_LIGHT_VTG 36
#define DEFAULT_MEDIUM_VTG 128
#define DEFAULT_STRONG_VTG 256

namespace {

static int get(std::string path, int defaultValue) {
    int value = defaultValue;
    std::ifstream file(path);

    if (!file) {
        ALOGE("Failed to open %s", path.c_str());
        return value;
    }

    file >> value;

    if (!file) {
        ALOGE("Failed to read value from %s", path.c_str());
    }

    return value;
}

static int set(std::string path, int value) {
    std::ofstream file(path);

    if (!file) {
        ALOGE("Failed to open %s", path.c_str());
        return -1;
    }

    file << value;

    if (!file) {
        ALOGE("Failed to write %d to %s", value, path.c_str());
        return -1;
    }

    return 0;
}

} // anonymous namespace

Vibrator::Vibrator() {
    defaultVoltage = get(VIBRATOR VTG_DEFAULT, DEFAULT_MAX_VTG);
    minVoltage = get(VIBRATOR VTG_MIN, DEFAULT_MIN_VTG);
    maxVoltage = get(VIBRATOR VTG_MAX, DEFAULT_MAX_VTG);

    lightVoltage = get(VIBRATOR VTG_LIGHT, DEFAULT_LIGHT_VTG);
    mediumVoltage = get(VIBRATOR VTG_MEDIUM, DEFAULT_MEDIUM_VTG);
    strongVoltage = get(VIBRATOR VTG_STRONG, DEFAULT_STRONG_VTG);

    defaultIntensity = intensity = 255 * defaultVoltage / maxVoltage;
}

// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
Return<Status> Vibrator::on(uint32_t timeout_ms) {
    if (set(VIBRATOR ENABLE, timeout_ms)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

Return<Status> Vibrator::off()  {
    if (set(VIBRATOR ENABLE, 0)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl()  {
    return true;
}

Return<Status> Vibrator::setVoltage(uint32_t voltage) {
    uint32_t scaledVoltage = voltage * intensity / 255;

    if (set(VIBRATOR VTG_LEVEL, scaledVoltage)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
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

    ALOGI("Voltage set to: %u", voltage);

    return setVoltage(voltage);
}

Return<void> Vibrator::perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    switch (effect) {
    case Effect::CLICK:
        uint32_t voltage;

        switch (strength) {
        case EffectStrength::LIGHT:
            voltage = lightVoltage;
            break;
        case EffectStrength::MEDIUM:
            voltage = mediumVoltage;
            break;
        case EffectStrength::STRONG:
            voltage = strongVoltage;
            break;
        default:
            _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
            return Void();
        }

        on(CLICK_TIMING_MS);
        setVoltage(voltage);
        _hidl_cb(Status::OK, CLICK_TIMING_MS);
        break;
    default:
        _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
        break;
    }

    return Void();
}

// Methods from IVibrator follow.
Return<void> Vibrator::getDefaultIntensity(getDefaultIntensity_cb _hidl_cb) {
    _hidl_cb(Status::OK, defaultIntensity);
    return Void();
}

Return<void> Vibrator::getIntensity(getIntensity_cb _hidl_cb) {
    _hidl_cb(Status::OK, intensity);
    return Void();
}

Return<Status> Vibrator::setIntensity(uint8_t newIntensity) {
    intensity = newIntensity;
    return Status::OK;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor
