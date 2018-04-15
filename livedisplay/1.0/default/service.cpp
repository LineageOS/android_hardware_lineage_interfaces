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

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::OK;
using android::sp;
using android::status_t;

using ::vendor::lineage::livedisplay::V1_0::IColor;
using ::vendor::lineage::livedisplay::V1_0::implementation::Color;

int main() {
    status_t status;
    android::sp<IColor> service = nullptr;

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    // The LiveDisplay HAL may communicate to other vendor components via /dev/vndbinder
    android::ProcessState::initWithDriver("/dev/vndbinder");

    service = Color::getInstance();
    if (service == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status = service->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for LiveDisplay HAL Iface (" << status << ")";
        goto shutdown;
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to exit
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}
