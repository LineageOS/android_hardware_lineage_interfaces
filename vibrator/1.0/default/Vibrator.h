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
#ifndef VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H
#define VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H

#include <vendor/lineage/vibrator/1.0/IVibrator.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace vibrator {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::vibrator::V1_0::Status;
using ::android::hardware::vibrator::V1_0::Effect;
using ::android::hardware::vibrator::V1_0::EffectStrength;

class Vibrator : public IVibrator {
public:
  Vibrator();
  Return<Status> setVoltage(uint32_t voltage);

  // Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
  Return<Status> on(uint32_t timeoutMs) override;
  Return<Status> off() override;
  Return<bool> supportsAmplitudeControl() override;
  Return<Status> setAmplitude(uint8_t amplitude) override;
  Return<void> perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) override;

  // Methods from IVibrator follow.
  Return<uint32_t> getDefaultIntensity() override;
  Return<void> setIntensity(uint32_t intensity) override;

private:
  uint32_t defaultVoltage;
  uint32_t minVoltage;
  uint32_t maxVoltage;

  uint8_t lightAmplitude;
  uint8_t mediumAmplitude;
  uint8_t strongAmplitude;

  uint32_t defaultIntensity;
  uint32_t intensity;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H
