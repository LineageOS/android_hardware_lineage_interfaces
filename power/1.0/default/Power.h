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


#ifndef VENDOR_LINEAGE_POWER_V1_0_POWER_H
#define VENDOR_LINEAGE_POWER_V1_0_POWER_H

#include <vendor/lineage/power/1.0/IPower.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace power {
namespace V1_0 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::PowerHint;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;

using ::vendor::lineage::power::V1_0::IPower;
using ::vendor::lineage::power::V1_0::Profile;
using ::vendor::lineage::power::V1_0::SupportedProfiles;

using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Power : public IPower {
    Power(power_module_t* module);
    ~Power();

    // Methods from ::android::hardware::power::V1_0::IPower follow.
    Return<void> setInteractive(bool interactive) override;
    Return<void> powerHint(PowerHint hint, int32_t data) override;
    Return<void> setFeature(Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

    // Methods from ::vendor::lineage::power::V1_0::IPower follow.
    Return<SupportedProfiles> getSupportedProfilesCount() override;
    Return<void> setProfile(Profile profile) override;
    Return<void> boost(uint32_t duration) override;

private:
	power_module_t* mModule;
};

extern "C" IPower* HIDL_FETCH_IPower(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_POWER_V1_0_POWER_H
