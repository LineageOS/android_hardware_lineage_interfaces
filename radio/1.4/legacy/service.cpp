/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.radio@1.4-service.legacy"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "Radio.h"

#include <map>

using namespace android::hardware::radio;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::radio::implementation::Radio;

using android::OK;
using android::sp;
using android::status_t;

#define MAX_SLOT_ID 4

sp<V1_0::IRadio> getRealRadio(int slotId) {
    sp<V1_0::IRadio> realRadio = nullptr;
    std::string serviceName = "slot" + std::to_string(slotId);

    realRadio = V1_4::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = V1_3::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = V1_2::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    realRadio = V1_1::IRadio::getService(serviceName);
    if (realRadio != nullptr) {
        return realRadio;
    }

    return V1_0::IRadio::getService(serviceName);
}

int main() {
    // Note: Starts from slot 1
    std::map<int, sp<V1_4::IRadio>> slotIdToRadio;

    for (int slotId = 1; slotId <= MAX_SLOT_ID; slotId++) {
        sp<V1_0::IRadio> realRadio = getRealRadio(slotId);
        if (realRadio == nullptr) {
            LOG(INFO) << "Cannot get radio service for slot " << slotId;

            if (slotId == 1) {
                LOG(ERROR) << "Cannot get radio service for slot 1.";
                return 1;
            }

            break;
        }

        // TODO: Use linkToDeath to monitor realRadio.

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
