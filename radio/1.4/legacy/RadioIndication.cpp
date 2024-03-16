/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioIndication.h"

namespace android::hardware::radio::implementation {

// Methods from ::android::hardware::radio::V1_0::IRadioIndication follow.
Return<void> RadioIndication::radioStateChanged(V1_0::RadioIndicationType type,
                                                V1_0::RadioState radioState) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::callStateChanged(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkStateChanged(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newSms(V1_0::RadioIndicationType type, const hidl_vec<uint8_t>& pdu) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newSmsStatusReport(V1_0::RadioIndicationType type,
                                                 const hidl_vec<uint8_t>& pdu) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newSmsOnSim(V1_0::RadioIndicationType type, int32_t recordNumber) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::onUssd(V1_0::RadioIndicationType type, V1_0::UssdModeType modeType,
                                     const hidl_string& msg) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::nitzTimeReceived(V1_0::RadioIndicationType type,
                                               const hidl_string& nitzTime, uint64_t receivedTime) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength(V1_0::RadioIndicationType type,
                                                    const V1_0::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::dataCallListChanged(
        V1_0::RadioIndicationType type, const hidl_vec<V1_0::SetupDataCallResult>& dcList) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::suppSvcNotify(V1_0::RadioIndicationType type,
                                            const V1_0::SuppSvcNotification& suppSvc) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkSessionEnd(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkProactiveCommand(V1_0::RadioIndicationType type,
                                                  const hidl_string& cmd) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkEventNotify(V1_0::RadioIndicationType type,
                                             const hidl_string& cmd) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkCallSetup(V1_0::RadioIndicationType type, int64_t timeout) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::simSmsStorageFull(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::simRefresh(V1_0::RadioIndicationType type,
                                         const V1_0::SimRefreshResult& refreshResult) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::callRing(V1_0::RadioIndicationType type, bool isGsm,
                                       const V1_0::CdmaSignalInfoRecord& record) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::simStatusChanged(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaNewSms(V1_0::RadioIndicationType type,
                                         const V1_0::CdmaSmsMessage& msg) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::newBroadcastSms(V1_0::RadioIndicationType type,
                                              const hidl_vec<uint8_t>& data) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaRuimSmsStorageFull(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::restrictedStateChanged(V1_0::RadioIndicationType type,
                                                     V1_0::PhoneRestrictedState state) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::enterEmergencyCallbackMode(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaCallWaiting(V1_0::RadioIndicationType type,
                                              const V1_0::CdmaCallWaiting& callWaitingRecord) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaOtaProvisionStatus(V1_0::RadioIndicationType type,
                                                     V1_0::CdmaOtaProvisionStatus status) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaInfoRec(V1_0::RadioIndicationType type,
                                          const V1_0::CdmaInformationRecords& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::indicateRingbackTone(V1_0::RadioIndicationType type, bool start) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::resendIncallMute(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaSubscriptionSourceChanged(
        V1_0::RadioIndicationType type, V1_0::CdmaSubscriptionSource cdmaSource) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cdmaPrlChanged(V1_0::RadioIndicationType type, int32_t version) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::exitEmergencyCallbackMode(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::rilConnected(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::voiceRadioTechChanged(V1_0::RadioIndicationType type,
                                                    V1_0::RadioTechnology rat) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList(V1_0::RadioIndicationType type,
                                           const hidl_vec<V1_0::CellInfo>& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::imsNetworkStateChanged(V1_0::RadioIndicationType type) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::subscriptionStatusChanged(V1_0::RadioIndicationType type,
                                                        bool activate) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::srvccStateNotify(V1_0::RadioIndicationType type,
                                               V1_0::SrvccState state) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::hardwareConfigChanged(V1_0::RadioIndicationType type,
                                                    const hidl_vec<V1_0::HardwareConfig>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::radioCapabilityIndication(V1_0::RadioIndicationType type,
                                                        const V1_0::RadioCapability& rc) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::onSupplementaryServiceIndication(V1_0::RadioIndicationType type,
                                                               const V1_0::StkCcUnsolSsResult& ss) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::stkCallControlAlphaNotify(V1_0::RadioIndicationType type,
                                                        const hidl_string& alpha) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::lceData(V1_0::RadioIndicationType type,
                                      const V1_0::LceDataInfo& lce) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::pcoData(V1_0::RadioIndicationType type,
                                      const V1_0::PcoDataInfo& pco) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::modemReset(V1_0::RadioIndicationType type,
                                         const hidl_string& reason) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_1::IRadioIndication follow.
Return<void> RadioIndication::carrierInfoForImsiEncryption(V1_0::RadioIndicationType info) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkScanResult(V1_0::RadioIndicationType type,
                                                const V1_1::NetworkScanResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::keepaliveStatus(V1_0::RadioIndicationType type,
                                              const V1_1::KeepaliveStatus& status) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadioIndication follow.
Return<void> RadioIndication::networkScanResult_1_2(V1_0::RadioIndicationType type,
                                                    const V1_2::NetworkScanResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList_1_2(V1_0::RadioIndicationType type,
                                               const hidl_vec<V1_2::CellInfo>& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentLinkCapacityEstimate(V1_0::RadioIndicationType type,
                                                          const V1_2::LinkCapacityEstimate& lce) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentPhysicalChannelConfigs(
        V1_0::RadioIndicationType type, const hidl_vec<V1_2::PhysicalChannelConfig>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength_1_2(
        V1_0::RadioIndicationType type, const V1_2::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadioIndication follow.
Return<void> RadioIndication::currentEmergencyNumberList(
        V1_0::RadioIndicationType type,
        const hidl_vec<V1_4::EmergencyNumber>& emergencyNumberList) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList_1_4(V1_0::RadioIndicationType type,
                                               const hidl_vec<V1_4::CellInfo>& records) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkScanResult_1_4(V1_0::RadioIndicationType type,
                                                    const V1_4::NetworkScanResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentPhysicalChannelConfigs_1_4(
        V1_0::RadioIndicationType type, const hidl_vec<V1_4::PhysicalChannelConfig>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::dataCallListChanged_1_4(
        V1_0::RadioIndicationType type, const hidl_vec<V1_4::SetupDataCallResult>& dcList) {
    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength_1_4(
        V1_0::RadioIndicationType type, const V1_4::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

}  // namespace android::hardware::radio::implementation
