/*
 * Copyright (C) 2018 The LineageOS Project
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

#define LOG_TAG "vendor.lineage.power@1.0-impl"

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "LineagePower.h"

namespace vendor {
namespace lineage {
namespace power {
namespace V1_0 {
namespace implementation {

LineagePower::LineagePower(power_module_t *module) : mModule(module) {
    if (mModule)
        mModule->init(mModule);
}

LineagePower::~LineagePower() {
    delete(mModule);
}

Return<int32_t> LineagePower::getFeature(LineageFeature feature)  {
    if (mModule->getFeature)
        return mModule->getFeature(mModule, static_cast<feature_t>(feature));
    return -1;
}

ILineagePower* HIDL_FETCH_ILineagePower(const char* /* name */) {
    const hw_module_t* hw_module = nullptr;
    power_module_t* power_module = nullptr;
    int err = hw_get_module(POWER_HARDWARE_MODULE_ID, &hw_module);
    if (err) {
        ALOGE("hw_get_module %s failed: %d", POWER_HARDWARE_MODULE_ID, err);
        return nullptr;
    }

    if (!hw_module->methods || !hw_module->methods->open) {
        power_module = reinterpret_cast<power_module_t*>(
            const_cast<hw_module_t*>(hw_module));
    } else {
        err = hw_module->methods->open(
            hw_module, POWER_HARDWARE_MODULE_ID,
            reinterpret_cast<hw_device_t**>(&power_module));
        if (err) {
            ALOGE("Passthrough failed to load legacy HAL.");
            return nullptr;
        }
    }
    return new LineagePower(power_module);
}

} // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace lineage
}  // namespace vendor
