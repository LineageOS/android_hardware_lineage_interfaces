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

#include <cutils/properties.h>
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

#define ENABLE      VIBRATOR "enable"

#define VTG_DEFAULT VIBRATOR "vtg_default"
#define VTG_LEVEL   VIBRATOR "vtg_level"
#define VTG_MIN     VIBRATOR "vtg_min"
#define VTG_MAX     VIBRATOR "vtg_max"

#define CLICK_TIMING_MS 20

#define DEFAULT_MIN_VTG 0
#define DEFAULT_MAX_VTG 255

#define AMPLITUDE "vendor.lineage.vibrator.amplitude."
#define AMPLITUDE_LIGHT    AMPLITUDE "light"
#define AMPLITUDE_MEDIUM   AMPLITUDE "medium"
#define AMPLITUDE_STRONG   AMPLITUDE "strong"

#define DEFAULT_LIGHT_AMPLITUDE     36
#define DEFAULT_MEDIUM_AMPLITUDE    128
#define DEFAULT_STRONG_AMPLITUDE    255

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
    defaultVoltage = get(VTG_DEFAULT, DEFAULT_MAX_VTG);
    minVoltage = get(VTG_MIN, DEFAULT_MIN_VTG);
    maxVoltage = get(VTG_MAX, DEFAULT_MAX_VTG);

    lightAmplitude = property_get_int32(AMPLITUDE_LIGHT, DEFAULT_LIGHT_AMPLITUDE);
    mediumAmplitude = property_get_int32(AMPLITUDE_MEDIUM, DEFAULT_MEDIUM_AMPLITUDE);
    strongAmplitude = property_get_int32(AMPLITUDE_STRONG, DEFAULT_STRONG_AMPLITUDE);

    defaultIntensity = intensity = 255 * defaultVoltage / maxVoltage;
}

// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
Return<Status> Vibrator::on(uint32_t timeout_ms) {
    if (set(ENABLE, timeout_ms)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

Return<Status> Vibrator::off()  {
    if (set(ENABLE, 0)) {
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl()  {
    return true;
}

Return<Status> Vibrator::setVoltage(uint32_t voltage) {
    uint32_t scaledVoltage = voltage * intensity / 255;

    if (set(VTG_LEVEL, scaledVoltage)) {
        return Status::UNKNOWN_ERROR;
    }

    ALOGI("Voltage set to: %u", voltage);

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

    return setVoltage(voltage);
}

Return<void> Vibrator::perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    switch (effect) {
    case Effect::CLICK:
        uint8_t amplitude;

        switch (strength) {
        case EffectStrength::LIGHT:
            amplitude = lightAmplitude;
            break;
        case EffectStrength::MEDIUM:
            amplitude = mediumAmplitude;
            break;
        case EffectStrength::STRONG:
            amplitude = strongAmplitude;
            break;
        default:
            _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
            return Void();
        }

        on(CLICK_TIMING_MS);
        setAmplitude(amplitude);
        _hidl_cb(Status::OK, CLICK_TIMING_MS);
        break;
    default:
        _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
        break;
    }

    return Void();
}

// Methods from IVibrator follow.
Return<uint8_t> Vibrator::getDefaultIntensity() {
    return defaultIntensity;
}

Return<uint8_t> Vibrator::getIntensity() {
    return intensity;
}

Return<void> Vibrator::setIntensity(uint8_t newIntensity) {
    intensity = newIntensity;
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor
