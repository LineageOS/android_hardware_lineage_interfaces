/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <lineage/hardware/radio/config/1.2/IRadioConfigResponse.h>

namespace lineage::hardware::radio::config::implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct RadioConfigResponse : public V1_2::IRadioConfigResponse {
    // Methods from ::lineage::hardware::radio::config::V1_0::IRadioConfigResponse follow.
    Return<void> getSimSlotsStatusResponse(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
            const hidl_vec<::lineage::hardware::radio::config::V1_0::SimSlotStatus>& slotStatus)
            override;
    Return<void> setSimSlotsMappingResponse(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info) override;

    // Methods from ::lineage::hardware::radio::config::V1_1::IRadioConfigResponse follow.
    Return<void> getPhoneCapabilityResponse(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
            const ::lineage::hardware::radio::config::V1_1::PhoneCapability& phoneCapability)
            override;
    Return<void> setPreferredDataModemResponse(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info) override;
    Return<void> setModemsConfigResponse(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info) override;
    Return<void> getModemsConfigResponse(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
            const ::lineage::hardware::radio::config::V1_1::ModemsConfig& modemsConfig) override;

    // Methods from ::lineage::hardware::radio::config::V1_2::IRadioConfigResponse follow.
    Return<void> getSimSlotsStatusResponse_1_2(
            const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
            const hidl_vec<::lineage::hardware::radio::config::V1_2::SimSlotStatus>& slotStatus)
            override;
};

}  // namespace lineage::hardware::radio::config::implementation
