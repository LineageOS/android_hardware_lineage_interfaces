/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioResponse.h"

namespace android::hardware::radio::implementation {

// Methods from ::android::hardware::radio::V1_0::IRadioResponse follow.
Return<void> RadioResponse::getIccCardStatusResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::CardStatus& cardStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPinForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPukForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPin2ForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyIccPuk2ForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::changeIccPinForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::changeIccPin2ForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::supplyNetworkDepersonalizationResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCurrentCallsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::Call>& calls) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::dialResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIMSIForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, const hidl_string& imsi) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::hangupConnectionResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::hangupWaitingOrBackgroundResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::hangupForegroundResumeBackgroundResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::switchWaitingOrHoldingAndActiveResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::conferenceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::rejectCallResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getLastCallFailCauseResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::LastCallFailCauseInfo& failCauseinfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::SignalStrength& sigStrength) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRegistrationStateResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::VoiceRegStateResult& voiceRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::DataRegStateResult& dataRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getOperatorResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_string& longName, const hidl_string& shortName, const hidl_string& numeric) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setRadioPowerResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendDtmfResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendSmsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendSMSExpectMoreResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setupDataCallResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::SetupDataCallResult& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccIOForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::IccIoResult& iccIo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendUssdResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::cancelPendingUssdResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getClirResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t n, int32_t m) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setClirResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCallForwardStatusResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::CallForwardInfo>& callForwardInfos) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCallForwardResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCallWaitingResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool enable,
        int32_t serviceClass) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCallWaitingResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeLastIncomingGsmSmsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acceptCallResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::deactivateDataCallResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getFacilityLockForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t response) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setFacilityLockForAppResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t retry) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setBarringPasswordResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getNetworkSelectionModeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool manual) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setNetworkSelectionModeAutomaticResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setNetworkSelectionModeManualResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAvailableNetworksResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::OperatorInfo>& networkInfos) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startDtmfResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopDtmfResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getBasebandVersionResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_string& version) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::separateConnectionResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setMuteResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getMuteResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool enable) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getClipResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        ::android::hardware::radio::V1_0::ClipStatus status) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataCallListResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::SetupDataCallResult>& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSuppServiceNotificationsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::writeSmsToSimResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t index) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::deleteSmsOnSimResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setBandModeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAvailableBandModesResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::RadioBandMode>& bandModes) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendEnvelopeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_string& commandResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendTerminalResponseToSimResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::handleStkCallSetupRequestFromSimResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::explicitCallTransferResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredNetworkTypeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredNetworkTypeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        ::android::hardware::radio::V1_0::PreferredNetworkType nwType) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getNeighboringCidsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::NeighboringCell>& cells) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setLocationUpdatesResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaSubscriptionSourceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaRoamingPreferenceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCdmaRoamingPreferenceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        ::android::hardware::radio::V1_0::CdmaRoamingType type) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setTTYModeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getTTYModeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        ::android::hardware::radio::V1_0::TtyMode mode) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredVoicePrivacyResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredVoicePrivacyResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool enable) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendCDMAFeatureCodeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendBurstDtmfResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendCdmaSmsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeLastIncomingCdmaSmsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getGsmBroadcastConfigResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::GsmBroadcastSmsConfigInfo>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setGsmBroadcastConfigResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setGsmBroadcastActivationResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCdmaBroadcastConfigResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::CdmaBroadcastSmsConfigInfo>& configs) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaBroadcastConfigResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCdmaBroadcastActivationResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCDMASubscriptionResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, const hidl_string& mdn,
        const hidl_string& hSid, const hidl_string& hNid, const hidl_string& min,
        const hidl_string& prl) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::writeSmsToRuimResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, uint32_t index) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::deleteSmsOnRuimResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDeviceIdentityResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, const hidl_string& imei,
        const hidl_string& imeisv, const hidl_string& esn, const hidl_string& meid) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::exitEmergencyCallbackModeResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSmscAddressResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, const hidl_string& smsc) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSmscAddressResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::reportSmsMemoryStatusResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::reportStkServiceIsRunningResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCdmaSubscriptionSourceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        ::android::hardware::radio::V1_0::CdmaSubscriptionSource source) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::requestIsimAuthenticationResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_string& response) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeIncomingGsmSmsWithPduResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendEnvelopeWithStatusResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::IccIoResult& iccIo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRadioTechnologyResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        ::android::hardware::radio::V1_0::RadioTechnology rat) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCellInfoListResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::CellInfo>& cellInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setCellInfoListRateResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setInitialAttachApnResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getImsRegistrationStateResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool isRegistered,
        ::android::hardware::radio::V1_0::RadioTechnologyFamily ratFamily) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendImsSmsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::SendSmsResult& sms) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccTransmitApduBasicChannelResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::IccIoResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccOpenLogicalChannelResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t channelId,
        const hidl_vec<int8_t>& selectResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccCloseLogicalChannelResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::iccTransmitApduLogicalChannelResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::IccIoResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvReadItemResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_string& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvWriteItemResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvWriteCdmaPrlResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::nvResetConfigResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setUiccSubscriptionResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setDataAllowedResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getHardwareConfigResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_0::HardwareConfig>& config) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::requestIccSimAuthenticationResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::IccIoResult& result) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setDataProfileResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::requestShutdownResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getRadioCapabilityResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::RadioCapability& rc) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setRadioCapabilityResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::RadioCapability& rc) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startLceServiceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::LceStatusInfo& statusInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopLceServiceResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::LceStatusInfo& statusInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::pullLceDataResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::LceDataInfo& lceInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getModemActivityInfoResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_0::ActivityStatsInfo& activityInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setAllowedCarriersResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, int32_t numAllowed) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAllowedCarriersResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool allAllowed,
        const ::android::hardware::radio::V1_0::CarrierRestrictions& carriers) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::sendDeviceStateResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setIndicationFilterResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSimCardPowerResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::acknowledgeRequest(int32_t serial) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_1::IRadioResponse follow.
Return<void> RadioResponse::setCarrierInfoForImsiEncryptionResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSimCardPowerResponse_1_1(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startNetworkScanResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopNetworkScanResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startKeepaliveResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_1::KeepaliveStatus& status) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopKeepaliveResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadioResponse follow.
Return<void> RadioResponse::getCellInfoListResponse_1_2(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_2::CellInfo>& cellInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIccCardStatusResponse_1_2(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_2::CardStatus& cardStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSignalStrengthReportingCriteriaResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setLinkCapacityReportingCriteriaResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCurrentCallsResponse_1_2(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_2::Call>& calls) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse_1_2(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_2::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRegistrationStateResponse_1_2(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_2::VoiceRegStateResult& voiceRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse_1_2(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_2::DataRegStateResult& dataRegResponse) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_3::IRadioResponse follow.
Return<void> RadioResponse::setSystemSelectionChannelsResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::enableModemResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getModemStackStatusResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info, bool isEnabled) {
    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadioResponse follow.
Return<void> RadioResponse::emergencyDialResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::startNetworkScanResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCellInfoListResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_4::CellInfo>& cellInfo) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_4::DataRegStateResult& dataRegResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIccCardStatusResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_4::CardStatus& cardStatus) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredNetworkTypeBitmapResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        hidl_bitfield<RadioAccessFamily> networkTypeBitmap) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredNetworkTypeBitmapResponse(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataCallListResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const hidl_vec<::android::hardware::radio::V1_4::SetupDataCallResult>& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setupDataCallResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_4::SetupDataCallResult& dcResponse) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::setAllowedCarriersResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAllowedCarriersResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_4::CarrierRestrictionsWithPriority& carriers,
        ::android::hardware::radio::V1_4::SimLockMultiSimPolicy multiSimPolicy) {
    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse_1_4(
        const ::android::hardware::radio::V1_0::RadioResponseInfo& info,
        const ::android::hardware::radio::V1_4::SignalStrength& signalStrength) {
    // TODO implement
    return Void();
}

}  // namespace android::hardware::radio::implementation
