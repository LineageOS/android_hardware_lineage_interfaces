/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/radio/1.0/IRadio.h>
#include <android/hardware/radio/config/1.0/IRadioConfig.h>
#include <android/hardware/radio/config/1.1/IRadioConfig.h>
#include <android/hardware/radio/config/1.1/IRadioConfigResponse.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <lineage/hardware/radio/oldcfg/1.1/IRadioOldcfg.h>
#include <lineage/hardware/radio/oldcfg/1.1/IRadioOldcfgIndication.h>
#include <lineage/hardware/radio/oldcfg/1.1/IRadioOldcfgResponse.h>

#include <map>
#include <mutex>

namespace android::hardware::radio::config::implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct RadioConfig : public V1_1::IRadioConfig {
  public:
    RadioConfig(sp<::lineage::hardware::radio::oldcfg::V1_0::IRadioOldcfg> radioOldcfg,
                sp<::lineage::hardware::radio::oldcfg::V1_1::IRadioOldcfg> radioOldcfgV1_1);

    // Methods from ::android::hardware::radio::config::V1_0::IRadioConfig follow.
    Return<void> setResponseFunctions(
            const sp<::android::hardware::radio::config::V1_0::IRadioConfigResponse>&
                    radioConfigResponse,
            const sp<::android::hardware::radio::config::V1_0::IRadioConfigIndication>&
                    radioConfigIndication) override;
    Return<void> getSimSlotsStatus(int32_t serial) override;
    Return<void> setSimSlotsMapping(int32_t serial, const hidl_vec<uint32_t>& slotMap) override;

    // Methods from ::android::hardware::radio::config::V1_1::IRadioConfig follow.
    Return<void> getPhoneCapability(int32_t serial) override;
    Return<void> setPreferredDataModem(int32_t serial, uint8_t modemId) override;
    Return<void> setModemsConfig(
            int32_t serial,
            const ::android::hardware::radio::config::V1_1::ModemsConfig& modemsConfig) override;
    Return<void> getModemsConfig(int32_t serial) override;

  private:
    sp<::lineage::hardware::radio::oldcfg::V1_0::IRadioOldcfg> mRadioOldcfg;
    sp<::lineage::hardware::radio::oldcfg::V1_1::IRadioOldcfg> mRadioOldcfgV1_1;

    sp<::android::hardware::radio::config::V1_0::IRadioConfigResponse> mRadioConfigResponse;
    sp<::android::hardware::radio::config::V1_1::IRadioConfigResponse> mRadioConfigResponseV1_1;

    std::map<uint8_t, sp<::android::hardware::radio::V1_0::IRadio>> mModemIdToRadioCache;

    std::mutex mMutex;

    // Helper methods follow.
    sp<::android::hardware::radio::V1_0::IRadio> getRadioForModemId(uint8_t modemId);
    sp<::android::hardware::radio::config::V1_1::IRadioConfigResponse> getRadioConfigResponseV1_1();
    ::android::hardware::radio::V1_0::RadioResponseInfo getUnimplementedResponseInfo(
            int32_t serial);
};

}  // namespace android::hardware::radio::config::implementation
