/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioResponse.h"

namespace android::hardware::radio::implementation {

// Methods from ::android::hardware::radio::V1_0::IRadioResponse follow.
Return<void> RadioResponse::getIccCardStatusResponse(const V1_0::RadioResponseInfo& info,
                                                     const V1_0::CardStatus& cardStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPinForAppResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPukForAppResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPin2ForAppResponse(const V1_0::RadioResponseInfo& info,
                                                        int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPuk2ForAppResponse(const V1_0::RadioResponseInfo& info,
                                                        int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::changeIccPinForAppResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::changeIccPin2ForAppResponse(const V1_0::RadioResponseInfo& info,
                                                        int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyNetworkDepersonalizationResponse(
        const V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCurrentCallsResponse(const V1_0::RadioResponseInfo& info,
                                                    const hidl_vec<V1_0::Call>& calls) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::dialResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIMSIForAppResponse(const V1_0::RadioResponseInfo& info,
                                                  const hidl_string& imsi) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::hangupConnectionResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::hangupWaitingOrBackgroundResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::hangupForegroundResumeBackgroundResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::switchWaitingOrHoldingAndActiveResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::conferenceResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::rejectCallResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getLastCallFailCauseResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::LastCallFailCauseInfo& failCauseinfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse(const V1_0::RadioResponseInfo& info,
                                                      const V1_0::SignalStrength& sigStrength) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRegistrationStateResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::VoiceRegStateResult& voiceRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::DataRegStateResult& dataRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getOperatorResponse(const V1_0::RadioResponseInfo& info,
                                                const hidl_string& longName,
                                                const hidl_string& shortName,
                                                const hidl_string& numeric) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setRadioPowerResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendDtmfResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendSmsResponse(const V1_0::RadioResponseInfo& info,
                                            const V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendSMSExpectMoreResponse(const V1_0::RadioResponseInfo& info,
                                                      const V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setupDataCallResponse(const V1_0::RadioResponseInfo& info,
                                                  const V1_0::SetupDataCallResult& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccIOForAppResponse(const V1_0::RadioResponseInfo& info,
                                                const V1_0::IccIoResult& iccIo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendUssdResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::cancelPendingUssdResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getClirResponse(const V1_0::RadioResponseInfo& info, int32_t n,
                                            int32_t m) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setClirResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCallForwardStatusResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::CallForwardInfo>& callForwardInfos) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCallForwardResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCallWaitingResponse(const V1_0::RadioResponseInfo& info, bool enable,
                                                   int32_t serviceClass) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCallWaitingResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeLastIncomingGsmSmsResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acceptCallResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::deactivateDataCallResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getFacilityLockForAppResponse(const V1_0::RadioResponseInfo& info,
                                                          int32_t response) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setFacilityLockForAppResponse(const V1_0::RadioResponseInfo& info,
                                                          int32_t retry) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setBarringPasswordResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getNetworkSelectionModeResponse(const V1_0::RadioResponseInfo& info,
                                                            bool manual) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setNetworkSelectionModeAutomaticResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setNetworkSelectionModeManualResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAvailableNetworksResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::OperatorInfo>& networkInfos) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startDtmfResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopDtmfResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getBasebandVersionResponse(const V1_0::RadioResponseInfo& info,
                                                       const hidl_string& version) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::separateConnectionResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setMuteResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getMuteResponse(const V1_0::RadioResponseInfo& info, bool enable) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getClipResponse(const V1_0::RadioResponseInfo& info,
                                            V1_0::ClipStatus status) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataCallListResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::SetupDataCallResult>& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSuppServiceNotificationsResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::writeSmsToSimResponse(const V1_0::RadioResponseInfo& info,
                                                  int32_t index) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::deleteSmsOnSimResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setBandModeResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAvailableBandModesResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::RadioBandMode>& bandModes) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendEnvelopeResponse(const V1_0::RadioResponseInfo& info,
                                                 const hidl_string& commandResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendTerminalResponseToSimResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::handleStkCallSetupRequestFromSimResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::explicitCallTransferResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredNetworkTypeResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredNetworkTypeResponse(const V1_0::RadioResponseInfo& info,
                                                            V1_0::PreferredNetworkType nwType) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getNeighboringCidsResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::NeighboringCell>& cells) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setLocationUpdatesResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaSubscriptionSourceResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaRoamingPreferenceResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCdmaRoamingPreferenceResponse(const V1_0::RadioResponseInfo& info,
                                                             V1_0::CdmaRoamingType type) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setTTYModeResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getTTYModeResponse(const V1_0::RadioResponseInfo& info,
                                               V1_0::TtyMode mode) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredVoicePrivacyResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredVoicePrivacyResponse(const V1_0::RadioResponseInfo& info,
                                                             bool enable) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendCDMAFeatureCodeResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendBurstDtmfResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendCdmaSmsResponse(const V1_0::RadioResponseInfo& info,
                                                const V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeLastIncomingCdmaSmsResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getGsmBroadcastConfigResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::GsmBroadcastSmsConfigInfo>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setGsmBroadcastConfigResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setGsmBroadcastActivationResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCdmaBroadcastConfigResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::CdmaBroadcastSmsConfigInfo>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaBroadcastConfigResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaBroadcastActivationResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCDMASubscriptionResponse(
        const V1_0::RadioResponseInfo& info, const hidl_string& mdn, const hidl_string& hSid,
        const hidl_string& hNid, const hidl_string& min, const hidl_string& prl) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::writeSmsToRuimResponse(const V1_0::RadioResponseInfo& info,
                                                   uint32_t index) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::deleteSmsOnRuimResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDeviceIdentityResponse(const V1_0::RadioResponseInfo& info,
                                                      const hidl_string& imei,
                                                      const hidl_string& imeisv,
                                                      const hidl_string& esn,
                                                      const hidl_string& meid) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::exitEmergencyCallbackModeResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSmscAddressResponse(const V1_0::RadioResponseInfo& info,
                                                   const hidl_string& smsc) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSmscAddressResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::reportSmsMemoryStatusResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::reportStkServiceIsRunningResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCdmaSubscriptionSourceResponse(const V1_0::RadioResponseInfo& info,
                                                              V1_0::CdmaSubscriptionSource source) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::requestIsimAuthenticationResponse(const V1_0::RadioResponseInfo& info,
                                                              const hidl_string& response) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeIncomingGsmSmsWithPduResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendEnvelopeWithStatusResponse(const V1_0::RadioResponseInfo& info,
                                                           const V1_0::IccIoResult& iccIo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRadioTechnologyResponse(const V1_0::RadioResponseInfo& info,
                                                            V1_0::RadioTechnology rat) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCellInfoListResponse(const V1_0::RadioResponseInfo& info,
                                                    const hidl_vec<V1_0::CellInfo>& cellInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCellInfoListRateResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setInitialAttachApnResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getImsRegistrationStateResponse(const V1_0::RadioResponseInfo& info,
                                                            bool isRegistered,
                                                            V1_0::RadioTechnologyFamily ratFamily) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendImsSmsResponse(const V1_0::RadioResponseInfo& info,
                                               const V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccTransmitApduBasicChannelResponse(const V1_0::RadioResponseInfo& info,
                                                                const V1_0::IccIoResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccOpenLogicalChannelResponse(const V1_0::RadioResponseInfo& info,
                                                          int32_t channelId,
                                                          const hidl_vec<int8_t>& selectResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccCloseLogicalChannelResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccTransmitApduLogicalChannelResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::IccIoResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvReadItemResponse(const V1_0::RadioResponseInfo& info,
                                               const hidl_string& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvWriteItemResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvWriteCdmaPrlResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvResetConfigResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setUiccSubscriptionResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setDataAllowedResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getHardwareConfigResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::HardwareConfig>& config) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::requestIccSimAuthenticationResponse(const V1_0::RadioResponseInfo& info,
                                                                const V1_0::IccIoResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setDataProfileResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::requestShutdownResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getRadioCapabilityResponse(const V1_0::RadioResponseInfo& info,
                                                       const V1_0::RadioCapability& rc) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setRadioCapabilityResponse(const V1_0::RadioResponseInfo& info,
                                                       const V1_0::RadioCapability& rc) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startLceServiceResponse(const V1_0::RadioResponseInfo& info,
                                                    const V1_0::LceStatusInfo& statusInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopLceServiceResponse(const V1_0::RadioResponseInfo& info,
                                                   const V1_0::LceStatusInfo& statusInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::pullLceDataResponse(const V1_0::RadioResponseInfo& info,
                                                const V1_0::LceDataInfo& lceInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getModemActivityInfoResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::ActivityStatsInfo& activityInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setAllowedCarriersResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t numAllowed) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAllowedCarriersResponse(const V1_0::RadioResponseInfo& info,
                                                       bool allAllowed,
                                                       const V1_0::CarrierRestrictions& carriers) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendDeviceStateResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setIndicationFilterResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSimCardPowerResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeRequest(int32_t serial) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_1::IRadioResponse follow.
Return<void> RadioResponse::setCarrierInfoForImsiEncryptionResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSimCardPowerResponse_1_1(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startNetworkScanResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopNetworkScanResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startKeepaliveResponse(const V1_0::RadioResponseInfo& info,
                                                   const V1_1::KeepaliveStatus& status) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopKeepaliveResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadioResponse follow.
Return<void> RadioResponse::getCellInfoListResponse_1_2(const V1_0::RadioResponseInfo& info,
                                                        const hidl_vec<V1_2::CellInfo>& cellInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIccCardStatusResponse_1_2(const V1_0::RadioResponseInfo& info,
                                                         const V1_2::CardStatus& cardStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSignalStrengthReportingCriteriaResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setLinkCapacityReportingCriteriaResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCurrentCallsResponse_1_2(const V1_0::RadioResponseInfo& info,
                                                        const hidl_vec<V1_2::Call>& calls) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse_1_2(
        const V1_0::RadioResponseInfo& info, const V1_2::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRegistrationStateResponse_1_2(
        const V1_0::RadioResponseInfo& info, const V1_2::VoiceRegStateResult& voiceRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse_1_2(
        const V1_0::RadioResponseInfo& info, const V1_2::DataRegStateResult& dataRegResponse) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_3::IRadioResponse follow.
Return<void> RadioResponse::setSystemSelectionChannelsResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::enableModemResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getModemStackStatusResponse(const V1_0::RadioResponseInfo& info,
                                                        bool isEnabled) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadioResponse follow.
Return<void> RadioResponse::emergencyDialResponse(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startNetworkScanResponse_1_4(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCellInfoListResponse_1_4(const V1_0::RadioResponseInfo& info,
                                                        const hidl_vec<V1_4::CellInfo>& cellInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse_1_4(
        const V1_0::RadioResponseInfo& info, const V1_4::DataRegStateResult& dataRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIccCardStatusResponse_1_4(const V1_0::RadioResponseInfo& info,
                                                         const V1_4::CardStatus& cardStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredNetworkTypeBitmapResponse(
        const V1_0::RadioResponseInfo& info, hidl_bitfield<V1_4::RadioAccessFamily> networkTypeBitmap) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredNetworkTypeBitmapResponse(
        const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataCallListResponse_1_4(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_4::SetupDataCallResult>& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setupDataCallResponse_1_4(const V1_0::RadioResponseInfo& info,
                                                      const V1_4::SetupDataCallResult& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setAllowedCarriersResponse_1_4(const V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAllowedCarriersResponse_1_4(
        const V1_0::RadioResponseInfo& info, const V1_4::CarrierRestrictionsWithPriority& carriers,
        V1_4::SimLockMultiSimPolicy multiSimPolicy) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse_1_4(
        const V1_0::RadioResponseInfo& info, const V1_4::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

}  // namespace android::hardware::radio::implementation
