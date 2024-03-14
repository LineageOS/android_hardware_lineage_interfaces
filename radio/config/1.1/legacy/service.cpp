/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.radio.config@1.1-service.legacy"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "RadioConfig.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::radio::config::implementation::RadioConfig;

using android::OK;
using android::sp;
using android::status_t;

int main() {
    sp<::android::hardware::radio::config::V1_0::IRadioConfig> realRadioConfig = nullptr;
    realRadioConfig = ::android::hardware::radio::config::V1_0::IRadioConfig::getService();
    if (realRadioConfig == nullptr) {
        LOG(ERROR) << "Cannot get radio config service.";
        return 1;
    }

    // TODO: Use linkToDeath to monitor realRadioConfig.

    android::sp<android::hardware::radio::config::V1_1::IRadioConfig> radioConfig =
            new RadioConfig(realRadioConfig);

    configureRpcThreadpool(1, true);

    status_t status = radioConfig->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Cannot register RadioConfig HAL service.";
        return 1;
    }

    LOG(INFO) << "RadioConfig HAL service ready.";

    joinRpcThreadpool();

    LOG(INFO) << "RadioConfig HAL service failed to join thread pool.";
    return 1;
}
