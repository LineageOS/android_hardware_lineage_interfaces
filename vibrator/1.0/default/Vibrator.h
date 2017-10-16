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

#ifndef VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H
#define VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H

#include <vendor/lineage/vibrator/1.0/IVibrator.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace vibrator {
namespace V1_0 {
namespace implementation {

using ::android::hardware::vibrator::V1_0::Effect;
using ::android::hardware::vibrator::V1_0::EffectStrength;
using ::android::hardware::vibrator::V1_0::Status;
using ::vendor::lineage::vibrator::V1_0::IVibrator;
using ::android::hardware::Return;
using ::android::hardware::Void;

class Vibrator : public IVibrator {
public:
    Vibrator();

    // Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
    Return<Status> on(uint32_t timeoutMs) override;
    Return<Status> off() override;
    Return<bool> supportsAmplitudeControl() override;
    Return<Status> setAmplitude(uint8_t amplitude) override;
    Return<void> perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) override;

    // Methods from ::vendor::lineage::vibrator::V1_0::IVibrator follow.
    Return<void> getDefaultAmplitude(getDefaultAmplitude_cb _hidl_cb) override;

private:
    uint32_t defaultVoltage;
    uint32_t minVoltage;
    uint32_t maxVoltage;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H
