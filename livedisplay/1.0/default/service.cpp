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

#define LOG_TAG "vendor.lineage.livedisplay@1.0-service"

#include <android/log.h>
#include <hidl/HidlTransportSupport.h>

#include "LiveDisplay.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::OK;
using android::sp;

using ::vendor::lineage::livedisplay::V1_0::ILiveDisplay;
using ::vendor::lineage::livedisplay::V1_0::implementation::LiveDisplay;


int main() {
    android::sp<LiveDisplay> ld = new LiveDisplay();

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (ld != nullptr) {
        if (ld->registerAsService() != OK) {
            ALOGE("Could not register service.");
            return 1;
        }
    } else {
        ALOGE("Can't create instance of LiveDisplay service, nullptr");
        return 1;
    }

    joinRpcThreadpool();

    return 0; // should never get here
}
