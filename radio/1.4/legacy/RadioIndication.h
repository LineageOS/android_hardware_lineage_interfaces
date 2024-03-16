/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/radio/1.4/IRadioIndication.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android::hardware::radio::implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct RadioIndication : public V1_4::IRadioIndication {
    // Methods from ::android::hardware::radio::V1_0::IRadioIndication follow.
    Return<void> radioStateChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            ::android::hardware::radio::V1_0::RadioState radioState) override;
    Return<void> callStateChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> networkStateChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> newSms(::android::hardware::radio::V1_0::RadioIndicationType type,
                        const hidl_vec<uint8_t>& pdu) override;
    Return<void> newSmsStatusReport(::android::hardware::radio::V1_0::RadioIndicationType type,
                                    const hidl_vec<uint8_t>& pdu) override;
    Return<void> newSmsOnSim(::android::hardware::radio::V1_0::RadioIndicationType type,
                             int32_t recordNumber) override;
    Return<void> onUssd(::android::hardware::radio::V1_0::RadioIndicationType type,
                        ::android::hardware::radio::V1_0::UssdModeType modeType,
                        const hidl_string& msg) override;
    Return<void> nitzTimeReceived(::android::hardware::radio::V1_0::RadioIndicationType type,
                                  const hidl_string& nitzTime, uint64_t receivedTime) override;
    Return<void> currentSignalStrength(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::SignalStrength& signalStrength) override;
    Return<void> dataCallListChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_0::SetupDataCallResult>& dcList) override;
    Return<void> suppSvcNotify(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::SuppSvcNotification& suppSvc) override;
    Return<void> stkSessionEnd(::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> stkProactiveCommand(::android::hardware::radio::V1_0::RadioIndicationType type,
                                     const hidl_string& cmd) override;
    Return<void> stkEventNotify(::android::hardware::radio::V1_0::RadioIndicationType type,
                                const hidl_string& cmd) override;
    Return<void> stkCallSetup(::android::hardware::radio::V1_0::RadioIndicationType type,
                              int64_t timeout) override;
    Return<void> simSmsStorageFull(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> simRefresh(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::SimRefreshResult& refreshResult) override;
    Return<void> callRing(
            ::android::hardware::radio::V1_0::RadioIndicationType type, bool isGsm,
            const ::android::hardware::radio::V1_0::CdmaSignalInfoRecord& record) override;
    Return<void> simStatusChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> cdmaNewSms(::android::hardware::radio::V1_0::RadioIndicationType type,
                            const ::android::hardware::radio::V1_0::CdmaSmsMessage& msg) override;
    Return<void> newBroadcastSms(::android::hardware::radio::V1_0::RadioIndicationType type,
                                 const hidl_vec<uint8_t>& data) override;
    Return<void> cdmaRuimSmsStorageFull(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> restrictedStateChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            ::android::hardware::radio::V1_0::PhoneRestrictedState state) override;
    Return<void> enterEmergencyCallbackMode(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> cdmaCallWaiting(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::CdmaCallWaiting& callWaitingRecord) override;
    Return<void> cdmaOtaProvisionStatus(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            ::android::hardware::radio::V1_0::CdmaOtaProvisionStatus status) override;
    Return<void> cdmaInfoRec(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::CdmaInformationRecords& records) override;
    Return<void> indicateRingbackTone(::android::hardware::radio::V1_0::RadioIndicationType type,
                                      bool start) override;
    Return<void> resendIncallMute(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> cdmaSubscriptionSourceChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            ::android::hardware::radio::V1_0::CdmaSubscriptionSource cdmaSource) override;
    Return<void> cdmaPrlChanged(::android::hardware::radio::V1_0::RadioIndicationType type,
                                int32_t version) override;
    Return<void> exitEmergencyCallbackMode(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> rilConnected(::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> voiceRadioTechChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            ::android::hardware::radio::V1_0::RadioTechnology rat) override;
    Return<void> cellInfoList(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_0::CellInfo>& records) override;
    Return<void> imsNetworkStateChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type) override;
    Return<void> subscriptionStatusChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type, bool activate) override;
    Return<void> srvccStateNotify(::android::hardware::radio::V1_0::RadioIndicationType type,
                                  ::android::hardware::radio::V1_0::SrvccState state) override;
    Return<void> hardwareConfigChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_0::HardwareConfig>& configs) override;
    Return<void> radioCapabilityIndication(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::RadioCapability& rc) override;
    Return<void> onSupplementaryServiceIndication(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_0::StkCcUnsolSsResult& ss) override;
    Return<void> stkCallControlAlphaNotify(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_string& alpha) override;
    Return<void> lceData(::android::hardware::radio::V1_0::RadioIndicationType type,
                         const ::android::hardware::radio::V1_0::LceDataInfo& lce) override;
    Return<void> pcoData(::android::hardware::radio::V1_0::RadioIndicationType type,
                         const ::android::hardware::radio::V1_0::PcoDataInfo& pco) override;
    Return<void> modemReset(::android::hardware::radio::V1_0::RadioIndicationType type,
                            const hidl_string& reason) override;

    // Methods from ::android::hardware::radio::V1_1::IRadioIndication follow.
    Return<void> carrierInfoForImsiEncryption(
            ::android::hardware::radio::V1_0::RadioIndicationType info) override;
    Return<void> networkScanResult(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_1::NetworkScanResult& result) override;
    Return<void> keepaliveStatus(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_1::KeepaliveStatus& status) override;

    // Methods from ::android::hardware::radio::V1_2::IRadioIndication follow.
    Return<void> networkScanResult_1_2(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_2::NetworkScanResult& result) override;
    Return<void> cellInfoList_1_2(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_2::CellInfo>& records) override;
    Return<void> currentLinkCapacityEstimate(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_2::LinkCapacityEstimate& lce) override;
    Return<void> currentPhysicalChannelConfigs(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_2::PhysicalChannelConfig>& configs)
            override;
    Return<void> currentSignalStrength_1_2(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_2::SignalStrength& signalStrength) override;

    // Methods from ::android::hardware::radio::V1_4::IRadioIndication follow.
    Return<void> currentEmergencyNumberList(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_4::EmergencyNumber>& emergencyNumberList)
            override;
    Return<void> cellInfoList_1_4(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_4::CellInfo>& records) override;
    Return<void> networkScanResult_1_4(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_4::NetworkScanResult& result) override;
    Return<void> currentPhysicalChannelConfigs_1_4(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_4::PhysicalChannelConfig>& configs)
            override;
    Return<void> dataCallListChanged_1_4(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::android::hardware::radio::V1_4::SetupDataCallResult>& dcList) override;
    Return<void> currentSignalStrength_1_4(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const ::android::hardware::radio::V1_4::SignalStrength& signalStrength) override;
};

}  // namespace android::hardware::radio::implementation
