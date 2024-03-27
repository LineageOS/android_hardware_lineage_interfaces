/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioConfig.h"

#include <vector>

namespace android::hardware::radio::config::implementation {

using ::android::hardware::radio::V1_0::IRadio;
using ::android::hardware::radio::V1_0::RadioError;
using ::android::hardware::radio::V1_0::RadioResponseInfo;
using ::android::hardware::radio::V1_0::RadioResponseType;

RadioConfig::RadioConfig(sp<::android::hardware::radio::config::V1_0::IRadioConfig> realRadioConfig)
    : mRealRadioConfig(realRadioConfig) {}

// Methods from ::android::hardware::radio::config::V1_0::IRadioConfig follow.
Return<void> RadioConfig::setResponseFunctions(
        const sp<::android::hardware::radio::config::V1_0::IRadioConfigResponse>&
                radioConfigResponse,
        const sp<::android::hardware::radio::config::V1_0::IRadioConfigIndication>&
                radioConfigIndication) {
    mRadioConfigResponse = radioConfigResponse;

    auto realRadioConfig = mRealRadioConfig;
    if (realRadioConfig == nullptr) {
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE);
    }

    return realRadioConfig->setResponseFunctions(radioConfigResponse, radioConfigIndication);
}

Return<void> RadioConfig::getSimSlotsStatus(int32_t serial) {
    auto realRadioConfig = mRealRadioConfig;
    if (realRadioConfig == nullptr) {
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE);
    }

    return realRadioConfig->getSimSlotsStatus(serial);
}

Return<void> RadioConfig::setSimSlotsMapping(int32_t serial, const hidl_vec<uint32_t>& slotMap) {
    auto realRadioConfig = mRealRadioConfig;
    if (realRadioConfig == nullptr) {
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE);
    }

    return realRadioConfig->setSimSlotsMapping(serial, slotMap);
}

// Methods from ::android::hardware::radio::config::V1_1::IRadioConfig follow.
Return<void> RadioConfig::getPhoneCapability(int32_t serial) {
    auto radioConfigResponseV1_1 = getRadioConfigResponseV1_1();
    if (radioConfigResponseV1_1 == nullptr) {
        return Void();
    }

    RadioResponseInfo radioResponseInfo = {
            RadioResponseType::SOLICITED,
            serial,
            RadioError::NONE,
    };

    /*
     * Simulates android.hardware.radio.config@1.0 behavior
     * Refer to convertHalPhoneCapability() on RILUtils.java
     */
    ::android::hardware::radio::config::V1_1::PhoneCapability phoneCapability = {
            .maxActiveData = 0,
            .maxActiveInternetData = 0,
            .isInternetLingeringSupported = 0,
            .logicalModemList = {},
    };

    radioConfigResponseV1_1->getPhoneCapabilityResponse(radioResponseInfo, phoneCapability);

    return Void();
}

Return<void> RadioConfig::setPreferredDataModem(int32_t serial, uint8_t modemId) {
    std::vector<sp<IRadio>> radios;

    uint8_t numSlots = 0;
    while (true) {
        auto radio = getRadioForModemId(numSlots);
        if (radio == nullptr) {
            break;
        }

        radios.push_back(radio);

        numSlots++;
    }

    RadioError radioError = RadioError::NONE;

    if (radios.empty() || modemId >= radios.size()) {
        radioError = RadioError::INVALID_ARGUMENTS;
    } else {
        for (uint8_t i = 0; i < radios.size(); i++) {
            auto radio = radios[i];
            if (radio == nullptr) {
                continue;
            }

            radio->setDataAllowed(-1, i == modemId);
        }
    }

    auto radioConfigResponseV1_1 = getRadioConfigResponseV1_1();

    if (radioConfigResponseV1_1 == nullptr) {
        return Void();
    }

    RadioResponseInfo radioResponseInfo = {
            RadioResponseType::SOLICITED,
            serial,
            radioError,
    };

    radioConfigResponseV1_1->setPreferredDataModemResponse(radioResponseInfo);

    return Void();
}

Return<void> RadioConfig::setModemsConfig(
        int32_t serial,
        const ::android::hardware::radio::config::V1_1::ModemsConfig& /*modemsConfig*/) {
    auto radioConfigResponseV1_1 = getRadioConfigResponseV1_1();
    if (radioConfigResponseV1_1 == nullptr) {
        return Void();
    }

    radioConfigResponseV1_1->setModemsConfigResponse(getUnimplementedResponseInfo(serial));

    return Void();
}

Return<void> RadioConfig::getModemsConfig(int32_t serial) {
    auto radioConfigResponseV1_1 = getRadioConfigResponseV1_1();
    if (radioConfigResponseV1_1 == nullptr) {
        return Void();
    }

    radioConfigResponseV1_1->getModemsConfigResponse(getUnimplementedResponseInfo(serial), {});

    return Void();
}

sp<IRadio> RadioConfig::getRadioForModemId(uint8_t modemId) {
    if (mModemIdToRadioCache.find(modemId) == mModemIdToRadioCache.end()) {
        mModemIdToRadioCache[modemId] = IRadio::getService("slot" + std::to_string(modemId + 1));
    }

    return mModemIdToRadioCache[modemId];
}

sp<::android::hardware::radio::config::V1_1::IRadioConfigResponse>
RadioConfig::getRadioConfigResponseV1_1() {
    return ::android::hardware::radio::config::V1_1::IRadioConfigResponse::castFrom(
                   mRadioConfigResponse)
            .withDefault(nullptr);
}

RadioResponseInfo RadioConfig::getUnimplementedResponseInfo(int32_t serial) {
    return {
            RadioResponseType::SOLICITED,
            serial,
            RadioError::REQUEST_NOT_SUPPORTED,
    };
}

}  // namespace android::hardware::radio::config::implementation
