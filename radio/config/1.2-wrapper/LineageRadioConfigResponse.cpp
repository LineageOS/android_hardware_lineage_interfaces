/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "RadioConfigResponseWrapper"

#include <android-base/logging.h>

#include "LineageRadioConfigResponse.h"

namespace lineage::hardware::radio::config::implementation {

using ::android::hardware::radio::V1_0::RadioResponseInfo;

// Methods from ::lineage::hardware::radio::config::V1_0::IRadioConfigResponse follow.
Return<void> RadioConfigResponse::getSimSlotsStatusResponse(
        const RadioResponseInfo& info,
        const hidl_vec<::lineage::hardware::radio::config::V1_0::SimSlotStatus>& slotStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioConfigResponse::setSimSlotsMappingResponse(const RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

// Methods from ::lineage::hardware::radio::config::V1_1::IRadioConfigResponse follow.
Return<void> RadioConfigResponse::getPhoneCapabilityResponse(
        const RadioResponseInfo& info,
        const ::lineage::hardware::radio::config::V1_1::PhoneCapability& phoneCapability) {
    // TODO implement
    return Void();
}

Return<void> RadioConfigResponse::setPreferredDataModemResponse(const RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioConfigResponse::setModemsConfigResponse(const RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioConfigResponse::getModemsConfigResponse(
        const RadioResponseInfo& info,
        const ::lineage::hardware::radio::config::V1_1::ModemsConfig& modemsConfig) {
    // TODO implement
    return Void();
}

// Methods from ::lineage::hardware::radio::config::V1_2::IRadioConfigResponse follow.
Return<void> RadioConfigResponse::getSimSlotsStatusResponse_1_2(
        const RadioResponseInfo& info,
        const hidl_vec<::lineage::hardware::radio::config::V1_2::SimSlotStatus>& slotStatus) {
    // TODO implement
    return Void();
}

}  // namespace lineage::hardware::radio::config::implementation
