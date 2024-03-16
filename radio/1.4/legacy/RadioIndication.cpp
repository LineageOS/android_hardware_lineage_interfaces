/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioIndication.h"

namespace android::hardware::radio::implementation {

// Methods from ::android::hardware::radio::V1_0::IRadioIndication follow.
Return<void> RadioIndication::radioStateChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        ::android::hardware::radio::V1_0::RadioState radioState) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::callStateChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkStateChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newSms(::android::hardware::radio::V1_0::RadioIndicationType type,
                                     const hidl_vec<uint8_t>& pdu) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newSmsStatusReport(
        ::android::hardware::radio::V1_0::RadioIndicationType type, const hidl_vec<uint8_t>& pdu) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newSmsOnSim(
        ::android::hardware::radio::V1_0::RadioIndicationType type, int32_t recordNumber) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::onUssd(::android::hardware::radio::V1_0::RadioIndicationType type,
                                     ::android::hardware::radio::V1_0::UssdModeType modeType,
                                     const hidl_string& msg) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::nitzTimeReceived(
        ::android::hardware::radio::V1_0::RadioIndicationType type, const hidl_string& nitzTime,
        uint64_t receivedTime) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::dataCallListChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_0::SetupDataCallResult>& dcList) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::suppSvcNotify(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::SuppSvcNotification& suppSvc) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkSessionEnd(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkProactiveCommand(
        ::android::hardware::radio::V1_0::RadioIndicationType type, const hidl_string& cmd) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkEventNotify(
        ::android::hardware::radio::V1_0::RadioIndicationType type, const hidl_string& cmd) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkCallSetup(
        ::android::hardware::radio::V1_0::RadioIndicationType type, int64_t timeout) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::simSmsStorageFull(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::simRefresh(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::SimRefreshResult& refreshResult) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::callRing(
        ::android::hardware::radio::V1_0::RadioIndicationType type, bool isGsm,
        const ::android::hardware::radio::V1_0::CdmaSignalInfoRecord& record) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::simStatusChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaNewSms(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::CdmaSmsMessage& msg) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newBroadcastSms(
        ::android::hardware::radio::V1_0::RadioIndicationType type, const hidl_vec<uint8_t>& data) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaRuimSmsStorageFull(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::restrictedStateChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        ::android::hardware::radio::V1_0::PhoneRestrictedState state) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::enterEmergencyCallbackMode(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaCallWaiting(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::CdmaCallWaiting& callWaitingRecord) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaOtaProvisionStatus(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        ::android::hardware::radio::V1_0::CdmaOtaProvisionStatus status) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaInfoRec(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::CdmaInformationRecords& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::indicateRingbackTone(
        ::android::hardware::radio::V1_0::RadioIndicationType type, bool start) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::resendIncallMute(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaSubscriptionSourceChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        ::android::hardware::radio::V1_0::CdmaSubscriptionSource cdmaSource) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaPrlChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type, int32_t version) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::exitEmergencyCallbackMode(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::rilConnected(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::voiceRadioTechChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        ::android::hardware::radio::V1_0::RadioTechnology rat) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_0::CellInfo>& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::imsNetworkStateChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::subscriptionStatusChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type, bool activate) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::srvccStateNotify(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        ::android::hardware::radio::V1_0::SrvccState state) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::hardwareConfigChanged(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_0::HardwareConfig>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::radioCapabilityIndication(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::RadioCapability& rc) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::onSupplementaryServiceIndication(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_0::StkCcUnsolSsResult& ss) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkCallControlAlphaNotify(
        ::android::hardware::radio::V1_0::RadioIndicationType type, const hidl_string& alpha) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::lceData(::android::hardware::radio::V1_0::RadioIndicationType type,
                                      const ::android::hardware::radio::V1_0::LceDataInfo& lce) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::pcoData(::android::hardware::radio::V1_0::RadioIndicationType type,
                                      const ::android::hardware::radio::V1_0::PcoDataInfo& pco) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::modemReset(::android::hardware::radio::V1_0::RadioIndicationType type,
                                         const hidl_string& reason) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_1::IRadioIndication follow.
Return<void> RadioIndication::carrierInfoForImsiEncryption(
        ::android::hardware::radio::V1_0::RadioIndicationType info) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkScanResult(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_1::NetworkScanResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::keepaliveStatus(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_1::KeepaliveStatus& status) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadioIndication follow.
Return<void> RadioIndication::networkScanResult_1_2(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_2::NetworkScanResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList_1_2(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_2::CellInfo>& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentLinkCapacityEstimate(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_2::LinkCapacityEstimate& lce) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentPhysicalChannelConfigs(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_2::PhysicalChannelConfig>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength_1_2(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_2::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadioIndication follow.
Return<void> RadioIndication::currentEmergencyNumberList(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_4::EmergencyNumber>& emergencyNumberList) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList_1_4(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_4::CellInfo>& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkScanResult_1_4(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_4::NetworkScanResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentPhysicalChannelConfigs_1_4(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_4::PhysicalChannelConfig>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::dataCallListChanged_1_4(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const hidl_vec<::android::hardware::radio::V1_4::SetupDataCallResult>& dcList) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength_1_4(
        ::android::hardware::radio::V1_0::RadioIndicationType type,
        const ::android::hardware::radio::V1_4::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

// IRadioIndication* HIDL_FETCH_IRadioIndication(const char* /* name */) {
// return new RadioIndication();
//}
//
}  // namespace android::hardware::radio::implementation
