/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioIndication.h"
#include "Helpers.h"

#define WRAP_V1_0_CALL(method, ...)                                            \
    do {                                                                       \
        auto realRadioIndication = mRealRadioIndication;                       \
        if (realRadioIndication != nullptr) {                                  \
            return realRadioIndication->method(__VA_ARGS__);                   \
        }                                                                      \
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE); \
    } while (0)

#define MAYBE_WRAP_V1_1_CALL(method, ...)                              \
    do {                                                               \
        auto realRadioIndication_V1_1 = getRealRadioIndication_V1_1(); \
        if (realRadioIndication_V1_1 != nullptr) {                     \
            return realRadioIndication_V1_1->method(__VA_ARGS__);      \
        }                                                              \
    } while (0)

#define MAYBE_WRAP_V1_2_CALL(method, ...)                              \
    do {                                                               \
        auto realRadioIndication_V1_2 = getRealRadioIndication_V1_2(); \
        if (realRadioIndication_V1_2 != nullptr) {                     \
            return realRadioIndication_V1_2->method(__VA_ARGS__);      \
        }                                                              \
    } while (0)

#define MAYBE_WRAP_V1_3_CALL(method, ...)                              \
    do {                                                               \
        auto realRadioIndication_V1_3 = getRealRadioIndication_V1_3(); \
        if (realRadioIndication_V1_3 != nullptr) {                     \
            return realRadioIndication_V1_3->method(__VA_ARGS__);      \
        }                                                              \
    } while (0)

#define MAYBE_WRAP_V1_4_CALL(method, ...)                              \
    do {                                                               \
        auto realRadioIndication_V1_4 = getRealRadioIndication_V1_4(); \
        if (realRadioIndication_V1_4 != nullptr) {                     \
            return realRadioIndication_V1_4->method(__VA_ARGS__);      \
        }                                                              \
    } while (0)

namespace android::hardware::radio::implementation {

// Methods from ::android::hardware::radio::V1_0::IRadioIndication follow.
Return<void> RadioIndication::radioStateChanged(V1_0::RadioIndicationType type,
                                                V1_0::RadioState radioState) {
    WRAP_V1_0_CALL(radioStateChanged, type, radioState);
}

Return<void> RadioIndication::callStateChanged(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(callStateChanged, type);
}

Return<void> RadioIndication::networkStateChanged(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(networkStateChanged, type);
}

Return<void> RadioIndication::newSms(V1_0::RadioIndicationType type, const hidl_vec<uint8_t>& pdu) {
    WRAP_V1_0_CALL(newSms, type, pdu);
}

Return<void> RadioIndication::newSmsStatusReport(V1_0::RadioIndicationType type,
                                                 const hidl_vec<uint8_t>& pdu) {
    WRAP_V1_0_CALL(newSmsStatusReport, type, pdu);
}

Return<void> RadioIndication::newSmsOnSim(V1_0::RadioIndicationType type, int32_t recordNumber) {
    WRAP_V1_0_CALL(newSmsOnSim, type, recordNumber);
}

Return<void> RadioIndication::onUssd(V1_0::RadioIndicationType type, V1_0::UssdModeType modeType,
                                     const hidl_string& msg) {
    WRAP_V1_0_CALL(onUssd, type, modeType, msg);
}

Return<void> RadioIndication::nitzTimeReceived(V1_0::RadioIndicationType type,
                                               const hidl_string& nitzTime, uint64_t receivedTime) {
    WRAP_V1_0_CALL(nitzTimeReceived, type, nitzTime, receivedTime);
}

Return<void> RadioIndication::currentSignalStrength(V1_0::RadioIndicationType type,
                                                    const V1_0::SignalStrength& signalStrength) {
    MAYBE_WRAP_V1_4_CALL(currentSignalStrength_1_4, type, Create1_4SignalStrength(signalStrength));

    WRAP_V1_0_CALL(currentSignalStrength, type, signalStrength);
}

Return<void> RadioIndication::dataCallListChanged(
        V1_0::RadioIndicationType type, const hidl_vec<V1_0::SetupDataCallResult>& dcList) {
    hidl_vec<V1_4::SetupDataCallResult> newDcList;
    newDcList.resize(dcList.size());
    for (int x = 0; x < dcList.size(); ++x) newDcList[x] = Create1_4SetupDataCallResult(dcList[x]);

    MAYBE_WRAP_V1_4_CALL(dataCallListChanged_1_4, type, newDcList);

    WRAP_V1_0_CALL(dataCallListChanged, type, dcList);
}

Return<void> RadioIndication::suppSvcNotify(V1_0::RadioIndicationType type,
                                            const V1_0::SuppSvcNotification& suppSvc) {
    WRAP_V1_0_CALL(suppSvcNotify, type, suppSvc);
}

Return<void> RadioIndication::stkSessionEnd(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(stkSessionEnd, type);
}

Return<void> RadioIndication::stkProactiveCommand(V1_0::RadioIndicationType type,
                                                  const hidl_string& cmd) {
    WRAP_V1_0_CALL(stkProactiveCommand, type, cmd);
}

Return<void> RadioIndication::stkEventNotify(V1_0::RadioIndicationType type,
                                             const hidl_string& cmd) {
    WRAP_V1_0_CALL(stkEventNotify, type, cmd);
}

Return<void> RadioIndication::stkCallSetup(V1_0::RadioIndicationType type, int64_t timeout) {
    WRAP_V1_0_CALL(stkCallSetup, type, timeout);
}

Return<void> RadioIndication::simSmsStorageFull(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(simSmsStorageFull, type);
}

Return<void> RadioIndication::simRefresh(V1_0::RadioIndicationType type,
                                         const V1_0::SimRefreshResult& refreshResult) {
    WRAP_V1_0_CALL(simRefresh, type, refreshResult);
}

Return<void> RadioIndication::callRing(V1_0::RadioIndicationType type, bool isGsm,
                                       const V1_0::CdmaSignalInfoRecord& record) {
    WRAP_V1_0_CALL(callRing, type, isGsm, record);
}

Return<void> RadioIndication::simStatusChanged(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(simStatusChanged, type);
}

Return<void> RadioIndication::cdmaNewSms(V1_0::RadioIndicationType type,
                                         const V1_0::CdmaSmsMessage& msg) {
    WRAP_V1_0_CALL(cdmaNewSms, type, msg);
}

Return<void> RadioIndication::newBroadcastSms(V1_0::RadioIndicationType type,
                                              const hidl_vec<uint8_t>& data) {
    WRAP_V1_0_CALL(newBroadcastSms, type, data);
}

Return<void> RadioIndication::cdmaRuimSmsStorageFull(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(cdmaRuimSmsStorageFull, type);
}

Return<void> RadioIndication::restrictedStateChanged(V1_0::RadioIndicationType type,
                                                     V1_0::PhoneRestrictedState state) {
    WRAP_V1_0_CALL(restrictedStateChanged, type, state);
}

Return<void> RadioIndication::enterEmergencyCallbackMode(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(enterEmergencyCallbackMode, type);
}

Return<void> RadioIndication::cdmaCallWaiting(V1_0::RadioIndicationType type,
                                              const V1_0::CdmaCallWaiting& callWaitingRecord) {
    WRAP_V1_0_CALL(cdmaCallWaiting, type, callWaitingRecord);
}

Return<void> RadioIndication::cdmaOtaProvisionStatus(V1_0::RadioIndicationType type,
                                                     V1_0::CdmaOtaProvisionStatus status) {
    WRAP_V1_0_CALL(cdmaOtaProvisionStatus, type, status);
}

Return<void> RadioIndication::cdmaInfoRec(V1_0::RadioIndicationType type,
                                          const V1_0::CdmaInformationRecords& records) {
    WRAP_V1_0_CALL(cdmaInfoRec, type, records);
}

Return<void> RadioIndication::indicateRingbackTone(V1_0::RadioIndicationType type, bool start) {
    WRAP_V1_0_CALL(indicateRingbackTone, type, start);
}

Return<void> RadioIndication::resendIncallMute(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(resendIncallMute, type);
}

Return<void> RadioIndication::cdmaSubscriptionSourceChanged(
        V1_0::RadioIndicationType type, V1_0::CdmaSubscriptionSource cdmaSource) {
    WRAP_V1_0_CALL(cdmaSubscriptionSourceChanged, type, cdmaSource);
}

Return<void> RadioIndication::cdmaPrlChanged(V1_0::RadioIndicationType type, int32_t version) {
    WRAP_V1_0_CALL(cdmaPrlChanged, type, version);
}

Return<void> RadioIndication::exitEmergencyCallbackMode(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(exitEmergencyCallbackMode, type);
}

Return<void> RadioIndication::rilConnected(V1_0::RadioIndicationType type) {
    WRAP_V1_0_CALL(rilConnected, type);
}

Return<void> RadioIndication::voiceRadioTechChanged(V1_0::RadioIndicationType type,
                                                    V1_0::RadioTechnology rat) {
    WRAP_V1_0_CALL(voiceRadioTechChanged, type, rat);
}

Return<void> RadioIndication::cellInfoList(V1_0::RadioIndicationType type,
                                           const hidl_vec<V1_0::CellInfo>& records) {
    MAYBE_WRAP_V1_4_CALL(cellInfoList_1_4, type, Create1_4CellInfoList(records));

    WRAP_V1_0_CALL(cellInfoList, type, records);
}

Return<void> RadioIndication::imsNetworkStateChanged(V1_0::RadioIndicationType type) {
    return mRealRadioIndication->imsNetworkStateChanged(type);
}

Return<void> RadioIndication::subscriptionStatusChanged(V1_0::RadioIndicationType type,
                                                        bool activate) {
    WRAP_V1_0_CALL(subscriptionStatusChanged, type, activate);
}

Return<void> RadioIndication::srvccStateNotify(V1_0::RadioIndicationType type,
                                               V1_0::SrvccState state) {
    WRAP_V1_0_CALL(srvccStateNotify, type, state);
}

Return<void> RadioIndication::hardwareConfigChanged(V1_0::RadioIndicationType type,
                                                    const hidl_vec<V1_0::HardwareConfig>& configs) {
    WRAP_V1_0_CALL(hardwareConfigChanged, type, configs);
}

Return<void> RadioIndication::radioCapabilityIndication(V1_0::RadioIndicationType type,
                                                        const V1_0::RadioCapability& rc) {
    WRAP_V1_0_CALL(radioCapabilityIndication, type, rc);
}

Return<void> RadioIndication::onSupplementaryServiceIndication(V1_0::RadioIndicationType type,
                                                               const V1_0::StkCcUnsolSsResult& ss) {
    WRAP_V1_0_CALL(onSupplementaryServiceIndication, type, ss);
}

Return<void> RadioIndication::stkCallControlAlphaNotify(V1_0::RadioIndicationType type,
                                                        const hidl_string& alpha) {
    WRAP_V1_0_CALL(stkCallControlAlphaNotify, type, alpha);
}

Return<void> RadioIndication::lceData(V1_0::RadioIndicationType type,
                                      const V1_0::LceDataInfo& lce) {
    WRAP_V1_0_CALL(lceData, type, lce);
}

Return<void> RadioIndication::pcoData(V1_0::RadioIndicationType type,
                                      const V1_0::PcoDataInfo& pco) {
    WRAP_V1_0_CALL(pcoData, type, pco);
}

Return<void> RadioIndication::modemReset(V1_0::RadioIndicationType type,
                                         const hidl_string& reason) {
    WRAP_V1_0_CALL(modemReset, type, reason);
}

// Methods from ::android::hardware::radio::V1_1::IRadioIndication follow.
Return<void> RadioIndication::carrierInfoForImsiEncryption(V1_0::RadioIndicationType info) {
    MAYBE_WRAP_V1_1_CALL(carrierInfoForImsiEncryption, info);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkScanResult(V1_0::RadioIndicationType type,
                                                const V1_1::NetworkScanResult& result) {
    V1_4::NetworkScanResult newNSR = {};
    newNSR.status = result.status;
    newNSR.error = result.error;
    newNSR.networkInfos = Create1_4CellInfoList(result.networkInfos);
    MAYBE_WRAP_V1_4_CALL(networkScanResult_1_4, type, newNSR);

    MAYBE_WRAP_V1_1_CALL(networkScanResult, type, result);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::keepaliveStatus(V1_0::RadioIndicationType type,
                                              const V1_1::KeepaliveStatus& status) {
    MAYBE_WRAP_V1_1_CALL(keepaliveStatus, type, status);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadioIndication follow.
Return<void> RadioIndication::networkScanResult_1_2(V1_0::RadioIndicationType type,
                                                    const V1_2::NetworkScanResult& result) {
    V1_4::NetworkScanResult newNSR = {};
    newNSR.status = result.status;
    newNSR.error = result.error;
    newNSR.networkInfos = Create1_4CellInfoList(result.networkInfos);
    MAYBE_WRAP_V1_4_CALL(networkScanResult_1_4, type, newNSR);

    MAYBE_WRAP_V1_2_CALL(networkScanResult_1_2, type, result);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList_1_2(V1_0::RadioIndicationType type,
                                               const hidl_vec<V1_2::CellInfo>& records) {
    MAYBE_WRAP_V1_4_CALL(cellInfoList_1_4, type, Create1_4CellInfoList(records));

    MAYBE_WRAP_V1_2_CALL(cellInfoList_1_2, type, records);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentLinkCapacityEstimate(V1_0::RadioIndicationType type,
                                                          const V1_2::LinkCapacityEstimate& lce) {
    MAYBE_WRAP_V1_2_CALL(currentLinkCapacityEstimate, type, lce);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentPhysicalChannelConfigs(
        V1_0::RadioIndicationType type, const hidl_vec<V1_2::PhysicalChannelConfig>& configs) {
    hidl_vec<V1_4::PhysicalChannelConfig> newConfigs;
    newConfigs.resize(configs.size());
    for (int x = 0; x < configs.size(); ++x) {
        newConfigs[x].base = configs[x];
        newConfigs[x].rat = V1_4::RadioTechnology::UNKNOWN;
        newConfigs[x].physicalCellId = -1;
    }
    MAYBE_WRAP_V1_4_CALL(currentPhysicalChannelConfigs_1_4, type, newConfigs);

    MAYBE_WRAP_V1_2_CALL(currentPhysicalChannelConfigs, type, configs);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength_1_2(
        V1_0::RadioIndicationType type, const V1_2::SignalStrength& signalStrength) {
    MAYBE_WRAP_V1_4_CALL(currentSignalStrength_1_4, type, Create1_4SignalStrength(signalStrength));

    MAYBE_WRAP_V1_2_CALL(currentSignalStrength_1_2, type, signalStrength);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadioIndication follow.
Return<void> RadioIndication::currentEmergencyNumberList(
        V1_0::RadioIndicationType type,
        const hidl_vec<V1_4::EmergencyNumber>& emergencyNumberList) {
    MAYBE_WRAP_V1_4_CALL(currentEmergencyNumberList, type, emergencyNumberList);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::cellInfoList_1_4(V1_0::RadioIndicationType type,
                                               const hidl_vec<V1_4::CellInfo>& records) {
    MAYBE_WRAP_V1_4_CALL(cellInfoList_1_4, type, records);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::networkScanResult_1_4(V1_0::RadioIndicationType type,
                                                    const V1_4::NetworkScanResult& result) {
    MAYBE_WRAP_V1_4_CALL(networkScanResult_1_4, type, result);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentPhysicalChannelConfigs_1_4(
        V1_0::RadioIndicationType type, const hidl_vec<V1_4::PhysicalChannelConfig>& configs) {
    MAYBE_WRAP_V1_4_CALL(currentPhysicalChannelConfigs_1_4, type, configs);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::dataCallListChanged_1_4(
        V1_0::RadioIndicationType type, const hidl_vec<V1_4::SetupDataCallResult>& dcList) {
    MAYBE_WRAP_V1_4_CALL(dataCallListChanged_1_4, type, dcList);

    // TODO implement
    return Void();
}

Return<void> RadioIndication::currentSignalStrength_1_4(
        V1_0::RadioIndicationType type, const V1_4::SignalStrength& signalStrength) {
    MAYBE_WRAP_V1_4_CALL(currentSignalStrength_1_4, type, signalStrength);

    // TODO implement
    return Void();
}

void RadioIndication::setRealRadioIndication(sp<V1_0::IRadioIndication> realRadioIndication) {
    mRealRadioIndication = realRadioIndication;
}

sp<V1_1::IRadioIndication> RadioIndication::getRealRadioIndication_V1_1() {
    return V1_1::IRadioIndication::castFrom(mRealRadioIndication).withDefault(nullptr);
}

sp<V1_2::IRadioIndication> RadioIndication::getRealRadioIndication_V1_2() {
    return V1_2::IRadioIndication::castFrom(mRealRadioIndication).withDefault(nullptr);
}

sp<V1_3::IRadioIndication> RadioIndication::getRealRadioIndication_V1_3() {
    return V1_3::IRadioIndication::castFrom(mRealRadioIndication).withDefault(nullptr);
}

sp<V1_4::IRadioIndication> RadioIndication::getRealRadioIndication_V1_4() {
    return V1_4::IRadioIndication::castFrom(mRealRadioIndication).withDefault(nullptr);
}

}  // namespace android::hardware::radio::implementation
