/*
 * Copyright (C) 2019 The LineageOS Project
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

#define LOG_TAG "vendor.qti.hardware.cryptfshw@1.0-service-qti"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "CryptfsHw.h"

using ::android::OK;
using ::android::sp;
using ::android::status_t;
using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;

using ::vendor::qti::hardware::cryptfshw::V1_0::implementation::CryptfsHw;

int main() {
    sp<CryptfsHw> cryptfsHw;
    status_t status = OK;

    LOG(INFO) << "CryptfsHw HAL service is starting.";

    cryptfsHw = new CryptfsHw();
    if (cryptfsHw == nullptr) {
        LOG(ERROR) << "Can not create an instance of CryptfsHw HAL CryptfsHw Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status = cryptfsHw->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for CryptfsHw HAL CryptfsHw Iface (" << status
                   << ")";
        goto shutdown;
    }

    LOG(INFO) << "CryptfsHw HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "CryptfsHw HAL service is shutting down.";
    return 1;
}
