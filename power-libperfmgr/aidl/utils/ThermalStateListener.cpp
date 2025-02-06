/*
 * Copyright 2025 The Android Open Source Project
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

#include "ThermalStateListener.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

bool ThermalStateListener::connectThermalHal() {
    const std::string thermalServiceName = std::string(IThermal::descriptor) + "/default";

    if (!AServiceManager_isDeclared(thermalServiceName.c_str())) {
        LOG(ERROR) << "Thermal HAL service not declared";
        return false;
    }

    ndk::SpAIBinder thermalHalBinder =
            ndk::SpAIBinder(AServiceManager_waitForService(thermalServiceName.c_str()));

    if (thermalHalBinder.get() == nullptr) {
        LOG(ERROR) << "Cannot get Thermal Hal binder!";
        return false;
    }

    mThermalAIDL = IThermal::fromBinder(thermalHalBinder);
    if (mThermalAIDL == nullptr) {
        LOG(ERROR) << "Cannot get Thermal Hal AIDL!";
        return false;
    }

    LOG(INFO) << "Connected to Thermalhal.";
    return true;
}

void ThermalStateListener::thermalCallback(const Temperature &temp) {
    if (temp.type == TemperatureType::SKIN) {
        LOG(INFO) << "New skin throttling state: "
                  << ::android::internal::ToString(temp.throttlingStatus);
        mThermalThrotSev = temp.throttlingStatus;
    }
}

bool ThermalStateListener::registerCallback() {
    if (mThermalAIDL == nullptr) {
        LOG(ERROR) << "Thermal Hal AIDL not connected!";
        return false;
    }

    if (mThermalCallback == nullptr) {
        mThermalCallback = ndk::SharedRefBase::make<ThermalCallback>(
                [this](const Temperature &temperature) { thermalCallback(temperature); });
    }

    auto ret = mThermalAIDL->registerThermalChangedCallback(mThermalCallback);
    if (!ret.isOk()) {
        LOG(ERROR) << "Failed to register the Powerhal's thermal callback: " << ret.getMessage();
        return false;
    }
    // TODO(guibing): handling the death connection
    LOG(INFO) << "Registered the thermal callback.";
    return true;
}

bool ThermalStateListener::init() {
    if (connectThermalHal() && registerCallback()) {
        return true;
    }
    LOG(ERROR) << "Failed to initialize the thermal state listener!";
    return false;
}

ThrottlingSeverity ThermalStateListener::getThermalThrotSev() {
    return mThermalThrotSev.load();
}

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
