/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.radio@1.4-service.wrapper"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include <lineage/hardware/radio/1.6/IRadio.h>

#include "Radio.h"
#include "hidl-utils.h"

#include <map>

using namespace android::hardware::hidl_utils;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::radio::implementation::Radio;

using android::OK;
using android::sp;
using android::status_t;

static constexpr int kMaxSlotId = 4;

static sp<lineage::hardware::radio::V1_0::IRadio> getRealRadio(int slotId) {
    sp<lineage::hardware::radio::V1_0::IRadio> realRadio = nullptr;
    std::string serviceName = "slot" + std::to_string(slotId);

    realRadio = lineage::hardware::radio::V1_6::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = lineage::hardware::radio::V1_5::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = lineage::hardware::radio::V1_4::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = lineage::hardware::radio::V1_3::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = lineage::hardware::radio::V1_2::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = lineage::hardware::radio::V1_1::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    return lineage::hardware::radio::V1_0::IRadio::getService(serviceName);
}

int main() {
    // Note: Starts from slot 1
    std::map<int, sp<android::hardware::radio::V1_4::IRadio>> slotIdToRadio;

    for (int slotId = 1; slotId <= kMaxSlotId; slotId++) {
        sp<lineage::hardware::radio::V1_0::IRadio> realRadio = getRealRadio(slotId);
        if (realRadio == nullptr) {
            LOG(INFO) << "Cannot get radio service for slot " << slotId;

            if (slotId == 1) {
                LOG(FATAL) << "Cannot get radio service for slot 1.";
            }

            break;
        }

        linkDeathToDeath(realRadio);

        slotIdToRadio[slotId] = new Radio(realRadio);
    }

    configureRpcThreadpool(1, true);

    for (auto const& [slotId, radio] : slotIdToRadio) {
        status_t status = radio->registerAsService("slot" + std::to_string(slotId));
        if (status != OK) {
            LOG(ERROR) << "Cannot register Radio HAL service for slot " << slotId;
            return 1;
        }
    }

    LOG(INFO) << "Radio HAL service ready.";

    joinRpcThreadpool();

    LOG(INFO) << "Radio HAL service failed to join thread pool.";
    return 1;
}
