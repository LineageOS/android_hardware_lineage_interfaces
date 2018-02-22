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

#ifdef COLOR_BACKEND_SDM
#define LOG_TAG "vendor.lineage.livedisplay@1.0-service-sdm"
#else
#error "Color backend undefined!"
#endif

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "Color.h"

using android::OK;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;

using ::vendor::lineage::livedisplay::V1_0::IColor;
using ::vendor::lineage::livedisplay::V1_0::implementation::Color;

int main() {
    android::sp<IColor> ld = Color::getInstance();

    // The livedisplay HAL may communicate to other vendor components via
    // /dev/vndbinder
    android::ProcessState::initWithDriver("/dev/vndbinder");

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (ld != nullptr) {
        if (ld->registerAsService() != OK) {
            LOG(ERROR) << "Could not register service.";
            return 1;
        }
    } else {
        LOG(ERROR) << "Can't create instance of LiveDisplay service, nullptr";
        return 1;
    }

    joinRpcThreadpool();

    return 0;  // should never get here
}
