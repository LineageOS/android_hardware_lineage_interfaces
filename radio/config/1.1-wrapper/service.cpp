/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.radio.config@1.1-service.wrapper"

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
    sp<lineage::hardware::radio::oldcfg::V1_0::IRadioOldcfg> radioOldcfg =
            lineage::hardware::radio::oldcfg::V1_0::IRadioOldcfg::getService();
    if (radioOldcfg == nullptr) {
        LOG(ERROR) << "Cannot get backend radio config V1.0 service (through IRadioOldcfg).";
        return 1;
    }

    sp<lineage::hardware::radio::oldcfg::V1_1::IRadioOldcfg> radioOldcfgV1_1 =
            lineage::hardware::radio::oldcfg::V1_1::IRadioOldcfg::getService();
    if (radioOldcfgV1_1 == nullptr) {
        LOG(ERROR) << "Cannot get backend radio config V1.1 service (through IRadioOldcfg) (not "
                      "fatal).";
    }

    // TODO: Use linkToDeath to monitor radioOldcfg.

    sp<android::hardware::radio::config::V1_1::IRadioConfig> radioConfig =
            new RadioConfig(radioOldcfg, radioOldcfgV1_1);

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
