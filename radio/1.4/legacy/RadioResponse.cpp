/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RadioResponse.h"
#include <string>
#include "Helpers.h"

#define WRAP_V1_0_CALL(method, ...)                                            \
    do {                                                                       \
        auto realRadioResponse = mRealRadioResponse;                           \
        if (realRadioResponse != nullptr) {                                    \
            return realRadioResponse->method(__VA_ARGS__);                     \
        }                                                                      \
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE); \
    } while (0)

#define MAYBE_WRAP_V1_1_CALL(method, ...)                          \
    do {                                                           \
        auto realRadioResponse_V1_1 = getRealRadioResponse_V1_1(); \
        if (realRadioResponse_V1_1 != nullptr) {                   \
            return realRadioResponse_V1_1->method(__VA_ARGS__);    \
        }                                                          \
    } while (0)

#define MAYBE_WRAP_V1_2_CALL(method, ...)                          \
    do {                                                           \
        auto realRadioResponse_V1_2 = getRealRadioResponse_V1_2(); \
        if (realRadioResponse_V1_2 != nullptr) {                   \
            return realRadioResponse_V1_2->method(__VA_ARGS__);    \
        }                                                          \
    } while (0)

#define MAYBE_WRAP_V1_3_CALL(method, ...)                          \
    do {                                                           \
        auto realRadioResponse_V1_3 = getRealRadioResponse_V1_3(); \
        if (realRadioResponse_V1_3 != nullptr) {                   \
            return realRadioResponse_V1_3->method(__VA_ARGS__);    \
        }                                                          \
    } while (0)

#define MAYBE_WRAP_V1_4_CALL(method, ...)                          \
    do {                                                           \
        auto realRadioResponse_V1_4 = getRealRadioResponse_V1_4(); \
        if (realRadioResponse_V1_4 != nullptr) {                   \
            return realRadioResponse_V1_4->method(__VA_ARGS__);    \
        }                                                          \
    } while (0)

namespace android::hardware::radio::implementation {

// Methods from ::android::hardware::radio::V1_0::IRadioResponse follow.
Return<void> RadioResponse::getIccCardStatusResponse(const V1_0::RadioResponseInfo& info,
                                                     const V1_0::CardStatus& cardStatus) {
    V1_4::CardStatus newCS = {};
    newCS.base.base = cardStatus;
    newCS.base.physicalSlotId = -1;
    newCS.base.iccid = hidl_string(" ");
    MAYBE_WRAP_V1_4_CALL(getIccCardStatusResponse_1_4, info, newCS);

    WRAP_V1_0_CALL(getIccCardStatusResponse, info, cardStatus);
}

Return<void> RadioResponse::supplyIccPinForAppResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t remainingRetries) {
    WRAP_V1_0_CALL(supplyIccPinForAppResponse, info, remainingRetries);
}

Return<void> RadioResponse::supplyIccPukForAppResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t remainingRetries) {
    WRAP_V1_0_CALL(supplyIccPukForAppResponse, info, remainingRetries);
}

Return<void> RadioResponse::supplyIccPin2ForAppResponse(const V1_0::RadioResponseInfo& info,
                                                        int32_t remainingRetries) {
    WRAP_V1_0_CALL(supplyIccPin2ForAppResponse, info, remainingRetries);
}

Return<void> RadioResponse::supplyIccPuk2ForAppResponse(const V1_0::RadioResponseInfo& info,
                                                        int32_t remainingRetries) {
    WRAP_V1_0_CALL(supplyIccPuk2ForAppResponse, info, remainingRetries);
}

Return<void> RadioResponse::changeIccPinForAppResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t remainingRetries) {
    WRAP_V1_0_CALL(changeIccPinForAppResponse, info, remainingRetries);
}

Return<void> RadioResponse::changeIccPin2ForAppResponse(const V1_0::RadioResponseInfo& info,
                                                        int32_t remainingRetries) {
    WRAP_V1_0_CALL(changeIccPin2ForAppResponse, info, remainingRetries);
}

Return<void> RadioResponse::supplyNetworkDepersonalizationResponse(
        const V1_0::RadioResponseInfo& info, int32_t remainingRetries) {
    WRAP_V1_0_CALL(supplyNetworkDepersonalizationResponse, info, remainingRetries);
}

Return<void> RadioResponse::getCurrentCallsResponse(const V1_0::RadioResponseInfo& info,
                                                    const hidl_vec<V1_0::Call>& calls) {
    hidl_vec<V1_2::Call> newCalls;
    newCalls.resize(calls.size());

    for (int x = 0; x < calls.size(); ++x) {
        newCalls[x].base = calls[x];
        newCalls[x].audioQuality = V1_2::AudioQuality::UNSPECIFIED;
    }
    MAYBE_WRAP_V1_2_CALL(getCurrentCallsResponse_1_2, info, newCalls);

    WRAP_V1_0_CALL(getCurrentCallsResponse, info, calls);
}

Return<void> RadioResponse::dialResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(dialResponse, info);
}

Return<void> RadioResponse::getIMSIForAppResponse(const V1_0::RadioResponseInfo& info,
                                                  const hidl_string& imsi) {
    WRAP_V1_0_CALL(getIMSIForAppResponse, info, imsi);
}

Return<void> RadioResponse::hangupConnectionResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(hangupConnectionResponse, info);
}

Return<void> RadioResponse::hangupWaitingOrBackgroundResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(hangupWaitingOrBackgroundResponse, info);
}

Return<void> RadioResponse::hangupForegroundResumeBackgroundResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(hangupForegroundResumeBackgroundResponse, info);
}

Return<void> RadioResponse::switchWaitingOrHoldingAndActiveResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(switchWaitingOrHoldingAndActiveResponse, info);
}

Return<void> RadioResponse::conferenceResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(conferenceResponse, info);
}

Return<void> RadioResponse::rejectCallResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(rejectCallResponse, info);
}

Return<void> RadioResponse::getLastCallFailCauseResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::LastCallFailCauseInfo& failCauseinfo) {
    WRAP_V1_0_CALL(getLastCallFailCauseResponse, info, failCauseinfo);
}

Return<void> RadioResponse::getSignalStrengthResponse(const V1_0::RadioResponseInfo& info,
                                                      const V1_0::SignalStrength& sigStrength) {
    MAYBE_WRAP_V1_4_CALL(getSignalStrengthResponse_1_4, info, Create1_4SignalStrength(sigStrength));

    WRAP_V1_0_CALL(getSignalStrengthResponse, info, sigStrength);
}

void Init1_2CellIdentity(const V1_0::CellIdentity& legacyCI, V1_2::CellIdentity& newCI) {
    if (legacyCI.cellIdentityGsm.size() == 1) {
        newCI.cellIdentityGsm.resize(1);
        newCI.cellIdentityGsm[0].base = legacyCI.cellIdentityGsm[0];
    } else if (legacyCI.cellIdentityCdma.size() == 1) {
        newCI.cellIdentityCdma.resize(1);
        newCI.cellIdentityCdma[0].base = legacyCI.cellIdentityCdma[0];
    } else if (legacyCI.cellIdentityLte.size() == 1) {
        newCI.cellIdentityLte.resize(1);
        newCI.cellIdentityLte[0].base = legacyCI.cellIdentityLte[0];
        newCI.cellIdentityLte[0].bandwidth = INT_MAX;
    } else if (legacyCI.cellIdentityWcdma.size() == 1) {
        newCI.cellIdentityWcdma.resize(1);
        newCI.cellIdentityWcdma[0].base = legacyCI.cellIdentityWcdma[0];
    } else if (legacyCI.cellIdentityTdscdma.size() == 1) {
        newCI.cellIdentityTdscdma.resize(1);
        newCI.cellIdentityTdscdma[0].base = legacyCI.cellIdentityTdscdma[0];
        newCI.cellIdentityTdscdma[0].uarfcn = INT_MAX;
    }
}

Return<void> RadioResponse::getVoiceRegistrationStateResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::VoiceRegStateResult& voiceRegResponse) {
    V1_2::VoiceRegStateResult newVRR = {};
    newVRR.regState = voiceRegResponse.regState;
    newVRR.rat = voiceRegResponse.rat;
    newVRR.cssSupported = voiceRegResponse.cssSupported;
    newVRR.roamingIndicator = voiceRegResponse.roamingIndicator;
    newVRR.systemIsInPrl = voiceRegResponse.systemIsInPrl;
    newVRR.defaultRoamingIndicator = voiceRegResponse.defaultRoamingIndicator;
    newVRR.reasonForDenial = voiceRegResponse.reasonForDenial;
    Init1_2CellIdentity(voiceRegResponse.cellIdentity, newVRR.cellIdentity);

    MAYBE_WRAP_V1_2_CALL(getVoiceRegistrationStateResponse_1_2, info, newVRR);

    WRAP_V1_0_CALL(getVoiceRegistrationStateResponse, info, voiceRegResponse);
}

Return<void> RadioResponse::getDataRegistrationStateResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::DataRegStateResult& dataRegResponse) {
    mDataRoaming = (dataRegResponse.regState == V1_0::RegState::REG_ROAMING);
    mRadioTechnology = (V1_0::RadioTechnology)dataRegResponse.rat;

    V1_4::DataRegStateResult newDRR = {};
    newDRR.base.regState = dataRegResponse.regState;
    newDRR.base.rat = dataRegResponse.rat;
    newDRR.base.reasonDataDenied = dataRegResponse.reasonDataDenied;
    newDRR.base.maxDataCalls = dataRegResponse.maxDataCalls;
    Init1_2CellIdentity(dataRegResponse.cellIdentity, newDRR.base.cellIdentity);

    MAYBE_WRAP_V1_4_CALL(getDataRegistrationStateResponse_1_4, info, newDRR);

    WRAP_V1_0_CALL(getDataRegistrationStateResponse, info, dataRegResponse);
}

Return<void> RadioResponse::getOperatorResponse(const V1_0::RadioResponseInfo& info,
                                                const hidl_string& longName,
                                                const hidl_string& shortName,
                                                const hidl_string& numeric) {
    WRAP_V1_0_CALL(getOperatorResponse, info, longName, shortName, numeric);
}

Return<void> RadioResponse::setRadioPowerResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setRadioPowerResponse, info);
}

Return<void> RadioResponse::sendDtmfResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(sendDtmfResponse, info);
}

Return<void> RadioResponse::sendSmsResponse(const V1_0::RadioResponseInfo& info,
                                            const V1_0::SendSmsResult& sms) {
    WRAP_V1_0_CALL(sendSmsResponse, info, sms);
}

Return<void> RadioResponse::sendSMSExpectMoreResponse(const V1_0::RadioResponseInfo& info,
                                                      const V1_0::SendSmsResult& sms) {
    WRAP_V1_0_CALL(sendSMSExpectMoreResponse, info, sms);
}

Return<void> RadioResponse::setupDataCallResponse(const V1_0::RadioResponseInfo& info,
                                                  const V1_0::SetupDataCallResult& dcResponse) {
    MAYBE_WRAP_V1_4_CALL(setupDataCallResponse_1_4, info, Create1_4SetupDataCallResult(dcResponse));

    WRAP_V1_0_CALL(setupDataCallResponse, info, dcResponse);
}

Return<void> RadioResponse::iccIOForAppResponse(const V1_0::RadioResponseInfo& info,
                                                const V1_0::IccIoResult& iccIo) {
    WRAP_V1_0_CALL(iccIOForAppResponse, info, iccIo);
}

Return<void> RadioResponse::sendUssdResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(sendUssdResponse, info);
}

Return<void> RadioResponse::cancelPendingUssdResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(cancelPendingUssdResponse, info);
}

Return<void> RadioResponse::getClirResponse(const V1_0::RadioResponseInfo& info, int32_t n,
                                            int32_t m) {
    WRAP_V1_0_CALL(getClirResponse, info, n, m);
}

Return<void> RadioResponse::setClirResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setClirResponse, info);
}

Return<void> RadioResponse::getCallForwardStatusResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::CallForwardInfo>& callForwardInfos) {
    WRAP_V1_0_CALL(getCallForwardStatusResponse, info, callForwardInfos);
}

Return<void> RadioResponse::setCallForwardResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCallForwardResponse, info);
}

Return<void> RadioResponse::getCallWaitingResponse(const V1_0::RadioResponseInfo& info, bool enable,
                                                   int32_t serviceClass) {
    WRAP_V1_0_CALL(getCallWaitingResponse, info, enable, serviceClass);
}

Return<void> RadioResponse::setCallWaitingResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCallWaitingResponse, info);
}

Return<void> RadioResponse::acknowledgeLastIncomingGsmSmsResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(acknowledgeLastIncomingGsmSmsResponse, info);
}

Return<void> RadioResponse::acceptCallResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(acceptCallResponse, info);
}

Return<void> RadioResponse::deactivateDataCallResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(deactivateDataCallResponse, info);
}

Return<void> RadioResponse::getFacilityLockForAppResponse(const V1_0::RadioResponseInfo& info,
                                                          int32_t response) {
    WRAP_V1_0_CALL(getFacilityLockForAppResponse, info, response);
}

Return<void> RadioResponse::setFacilityLockForAppResponse(const V1_0::RadioResponseInfo& info,
                                                          int32_t retry) {
    WRAP_V1_0_CALL(setFacilityLockForAppResponse, info, retry);
}

Return<void> RadioResponse::setBarringPasswordResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setBarringPasswordResponse, info);
}

Return<void> RadioResponse::getNetworkSelectionModeResponse(const V1_0::RadioResponseInfo& info,
                                                            bool manual) {
    WRAP_V1_0_CALL(getNetworkSelectionModeResponse, info, manual);
}

Return<void> RadioResponse::setNetworkSelectionModeAutomaticResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setNetworkSelectionModeAutomaticResponse, info);
}

Return<void> RadioResponse::setNetworkSelectionModeManualResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setNetworkSelectionModeManualResponse, info);
}

Return<void> RadioResponse::getAvailableNetworksResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::OperatorInfo>& networkInfos) {
    WRAP_V1_0_CALL(getAvailableNetworksResponse, info, networkInfos);
}

Return<void> RadioResponse::startDtmfResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(startDtmfResponse, info);
}

Return<void> RadioResponse::stopDtmfResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(stopDtmfResponse, info);
}

Return<void> RadioResponse::getBasebandVersionResponse(const V1_0::RadioResponseInfo& info,
                                                       const hidl_string& version) {
    WRAP_V1_0_CALL(getBasebandVersionResponse, info, version);
}

Return<void> RadioResponse::separateConnectionResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(separateConnectionResponse, info);
}

Return<void> RadioResponse::setMuteResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setMuteResponse, info);
}

Return<void> RadioResponse::getMuteResponse(const V1_0::RadioResponseInfo& info, bool enable) {
    WRAP_V1_0_CALL(getMuteResponse, info, enable);
}

Return<void> RadioResponse::getClipResponse(const V1_0::RadioResponseInfo& info,
                                            V1_0::ClipStatus status) {
    WRAP_V1_0_CALL(getClipResponse, info, status);
}

Return<void> RadioResponse::getDataCallListResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::SetupDataCallResult>& dcResponse) {
    hidl_vec<V1_4::SetupDataCallResult> newResponse;
    newResponse.resize(dcResponse.size());

    for (int x = 0; x < dcResponse.size(); ++x)
        newResponse[x] = Create1_4SetupDataCallResult(dcResponse[x]);

    MAYBE_WRAP_V1_4_CALL(getDataCallListResponse_1_4, info, newResponse);

    WRAP_V1_0_CALL(getDataCallListResponse, info, dcResponse);
}

Return<void> RadioResponse::setSuppServiceNotificationsResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setSuppServiceNotificationsResponse, info);
}

Return<void> RadioResponse::writeSmsToSimResponse(const V1_0::RadioResponseInfo& info,
                                                  int32_t index) {
    WRAP_V1_0_CALL(writeSmsToSimResponse, info, index);
}

Return<void> RadioResponse::deleteSmsOnSimResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(deleteSmsOnSimResponse, info);
}

Return<void> RadioResponse::setBandModeResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setBandModeResponse, info);
}

Return<void> RadioResponse::getAvailableBandModesResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::RadioBandMode>& bandModes) {
    WRAP_V1_0_CALL(getAvailableBandModesResponse, info, bandModes);
}

Return<void> RadioResponse::sendEnvelopeResponse(const V1_0::RadioResponseInfo& info,
                                                 const hidl_string& commandResponse) {
    WRAP_V1_0_CALL(sendEnvelopeResponse, info, commandResponse);
}

Return<void> RadioResponse::sendTerminalResponseToSimResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(sendTerminalResponseToSimResponse, info);
}

Return<void> RadioResponse::handleStkCallSetupRequestFromSimResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(handleStkCallSetupRequestFromSimResponse, info);
}

Return<void> RadioResponse::explicitCallTransferResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(explicitCallTransferResponse, info);
}

Return<void> RadioResponse::setPreferredNetworkTypeResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setPreferredNetworkTypeResponse, info);
}

Return<void> RadioResponse::getPreferredNetworkTypeResponse(const V1_0::RadioResponseInfo& info,
                                                            V1_0::PreferredNetworkType nwType) {
    hidl_bitfield<V1_4::RadioAccessFamily> nwTypeBitmap = 0;
    switch (nwType) {
        case V1_0::PreferredNetworkType::GSM_WCDMA:
        case V1_0::PreferredNetworkType::GSM_WCDMA_AUTO:
            nwTypeBitmap = GSMBITS | WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::GSM_ONLY:
            nwTypeBitmap = GSMBITS;
            break;
        case V1_0::PreferredNetworkType::WCDMA:
            nwTypeBitmap = WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::CDMA_EVDO_AUTO:
            nwTypeBitmap = CDMABITS | EVDOBITS;
            break;
        case V1_0::PreferredNetworkType::CDMA_ONLY:
            nwTypeBitmap = CDMABITS;
            break;
        case V1_0::PreferredNetworkType::EVDO_ONLY:
            nwTypeBitmap = EVDOBITS;
            break;
        case V1_0::PreferredNetworkType::GSM_WCDMA_CDMA_EVDO_AUTO:
            nwTypeBitmap = GSMBITS | WCDMABITS | CDMABITS | EVDOBITS;
            break;
        case V1_0::PreferredNetworkType::LTE_CDMA_EVDO:
            nwTypeBitmap = LTEBITS | CDMABITS | EVDOBITS;
            break;
        case V1_0::PreferredNetworkType::LTE_GSM_WCDMA:
            nwTypeBitmap = LTEBITS | GSMBITS | WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::LTE_CMDA_EVDO_GSM_WCDMA:
            nwTypeBitmap = LTEBITS | CDMABITS | EVDOBITS | GSMBITS | WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::LTE_ONLY:
            nwTypeBitmap = LTEBITS;
            break;
        case V1_0::PreferredNetworkType::LTE_WCDMA:
            nwTypeBitmap = LTEBITS | WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_ONLY:
            nwTypeBitmap = TDSCDMABIT;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_WCDMA:
            nwTypeBitmap = TDSCDMABIT | WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_LTE:
            nwTypeBitmap = TDSCDMABIT | LTEBITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_GSM:
            nwTypeBitmap = TDSCDMABIT | GSMBITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_GSM_LTE:
            nwTypeBitmap = TDSCDMABIT | GSMBITS | LTEBITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_GSM_WCDMA:
            nwTypeBitmap = TDSCDMABIT | GSMBITS | WCDMABITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_WCDMA_LTE:
            nwTypeBitmap = TDSCDMABIT | WCDMABITS | LTEBITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_GSM_WCDMA_LTE:
            nwTypeBitmap = TDSCDMABIT | GSMBITS | WCDMABITS | LTEBITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_GSM_WCDMA_CDMA_EVDO_AUTO:
            nwTypeBitmap = TDSCDMABIT | GSMBITS | WCDMABITS | CDMABITS | EVDOBITS;
            break;
        case V1_0::PreferredNetworkType::TD_SCDMA_LTE_CDMA_EVDO_GSM_WCDMA:
            nwTypeBitmap = TDSCDMABIT | LTEBITS | CDMABITS | EVDOBITS | GSMBITS | WCDMABITS;
            break;
    }

    MAYBE_WRAP_V1_4_CALL(getPreferredNetworkTypeBitmapResponse, info, nwTypeBitmap);

    WRAP_V1_0_CALL(getPreferredNetworkTypeResponse, info, nwType);
}

Return<void> RadioResponse::getNeighboringCidsResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::NeighboringCell>& cells) {
    WRAP_V1_0_CALL(getNeighboringCidsResponse, info, cells);
}

Return<void> RadioResponse::setLocationUpdatesResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setLocationUpdatesResponse, info);
}

Return<void> RadioResponse::setCdmaSubscriptionSourceResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCdmaSubscriptionSourceResponse, info);
}

Return<void> RadioResponse::setCdmaRoamingPreferenceResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCdmaRoamingPreferenceResponse, info);
}

Return<void> RadioResponse::getCdmaRoamingPreferenceResponse(const V1_0::RadioResponseInfo& info,
                                                             V1_0::CdmaRoamingType type) {
    WRAP_V1_0_CALL(getCdmaRoamingPreferenceResponse, info, type);
}

Return<void> RadioResponse::setTTYModeResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setTTYModeResponse, info);
}

Return<void> RadioResponse::getTTYModeResponse(const V1_0::RadioResponseInfo& info,
                                               V1_0::TtyMode mode) {
    WRAP_V1_0_CALL(getTTYModeResponse, info, mode);
}

Return<void> RadioResponse::setPreferredVoicePrivacyResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setPreferredVoicePrivacyResponse, info);
}

Return<void> RadioResponse::getPreferredVoicePrivacyResponse(const V1_0::RadioResponseInfo& info,
                                                             bool enable) {
    WRAP_V1_0_CALL(getPreferredVoicePrivacyResponse, info, enable);
}

Return<void> RadioResponse::sendCDMAFeatureCodeResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(sendCDMAFeatureCodeResponse, info);
}

Return<void> RadioResponse::sendBurstDtmfResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(sendBurstDtmfResponse, info);
}

Return<void> RadioResponse::sendCdmaSmsResponse(const V1_0::RadioResponseInfo& info,
                                                const V1_0::SendSmsResult& sms) {
    WRAP_V1_0_CALL(sendCdmaSmsResponse, info, sms);
}

Return<void> RadioResponse::acknowledgeLastIncomingCdmaSmsResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(acknowledgeLastIncomingCdmaSmsResponse, info);
}

Return<void> RadioResponse::getGsmBroadcastConfigResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::GsmBroadcastSmsConfigInfo>& configs) {
    WRAP_V1_0_CALL(getGsmBroadcastConfigResponse, info, configs);
}

Return<void> RadioResponse::setGsmBroadcastConfigResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setGsmBroadcastConfigResponse, info);
}

Return<void> RadioResponse::setGsmBroadcastActivationResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setGsmBroadcastActivationResponse, info);
}

Return<void> RadioResponse::getCdmaBroadcastConfigResponse(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_0::CdmaBroadcastSmsConfigInfo>& configs) {
    WRAP_V1_0_CALL(getCdmaBroadcastConfigResponse, info, configs);
}

Return<void> RadioResponse::setCdmaBroadcastConfigResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCdmaBroadcastConfigResponse, info);
}

Return<void> RadioResponse::setCdmaBroadcastActivationResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCdmaBroadcastActivationResponse, info);
}

Return<void> RadioResponse::getCDMASubscriptionResponse(
        const V1_0::RadioResponseInfo& info, const hidl_string& mdn, const hidl_string& hSid,
        const hidl_string& hNid, const hidl_string& min, const hidl_string& prl) {
    WRAP_V1_0_CALL(getCDMASubscriptionResponse, info, mdn, hSid, hNid, min, prl);
}

Return<void> RadioResponse::writeSmsToRuimResponse(const V1_0::RadioResponseInfo& info,
                                                   uint32_t index) {
    WRAP_V1_0_CALL(writeSmsToRuimResponse, info, index);
}

Return<void> RadioResponse::deleteSmsOnRuimResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(deleteSmsOnRuimResponse, info);
}

Return<void> RadioResponse::getDeviceIdentityResponse(const V1_0::RadioResponseInfo& info,
                                                      const hidl_string& imei,
                                                      const hidl_string& imeisv,
                                                      const hidl_string& esn,
                                                      const hidl_string& meid) {
    WRAP_V1_0_CALL(getDeviceIdentityResponse, info, imei, imeisv, esn, meid);
}

Return<void> RadioResponse::exitEmergencyCallbackModeResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(exitEmergencyCallbackModeResponse, info);
}

Return<void> RadioResponse::getSmscAddressResponse(const V1_0::RadioResponseInfo& info,
                                                   const hidl_string& smsc) {
    WRAP_V1_0_CALL(getSmscAddressResponse, info, smsc);
}

Return<void> RadioResponse::setSmscAddressResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setSmscAddressResponse, info);
}

Return<void> RadioResponse::reportSmsMemoryStatusResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(reportSmsMemoryStatusResponse, info);
}

Return<void> RadioResponse::reportStkServiceIsRunningResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(reportStkServiceIsRunningResponse, info);
}

Return<void> RadioResponse::getCdmaSubscriptionSourceResponse(const V1_0::RadioResponseInfo& info,
                                                              V1_0::CdmaSubscriptionSource source) {
    WRAP_V1_0_CALL(getCdmaSubscriptionSourceResponse, info, source);
}

Return<void> RadioResponse::requestIsimAuthenticationResponse(const V1_0::RadioResponseInfo& info,
                                                              const hidl_string& response) {
    WRAP_V1_0_CALL(requestIsimAuthenticationResponse, info, response);
}

Return<void> RadioResponse::acknowledgeIncomingGsmSmsWithPduResponse(
        const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(acknowledgeIncomingGsmSmsWithPduResponse, info);
}

Return<void> RadioResponse::sendEnvelopeWithStatusResponse(const V1_0::RadioResponseInfo& info,
                                                           const V1_0::IccIoResult& iccIo) {
    WRAP_V1_0_CALL(sendEnvelopeWithStatusResponse, info, iccIo);
}

Return<void> RadioResponse::getVoiceRadioTechnologyResponse(const V1_0::RadioResponseInfo& info,
                                                            V1_0::RadioTechnology rat) {
    WRAP_V1_0_CALL(getVoiceRadioTechnologyResponse, info, rat);
}

Return<void> RadioResponse::getCellInfoListResponse(const V1_0::RadioResponseInfo& info,
                                                    const hidl_vec<V1_0::CellInfo>& cellInfo) {
    MAYBE_WRAP_V1_4_CALL(getCellInfoListResponse_1_4, info, Create1_4CellInfoList(cellInfo));

    WRAP_V1_0_CALL(getCellInfoListResponse, info, cellInfo);
}

Return<void> RadioResponse::setCellInfoListRateResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setCellInfoListRateResponse, info);
}

Return<void> RadioResponse::setInitialAttachApnResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setInitialAttachApnResponse, info);
}

Return<void> RadioResponse::getImsRegistrationStateResponse(const V1_0::RadioResponseInfo& info,
                                                            bool isRegistered,
                                                            V1_0::RadioTechnologyFamily ratFamily) {
    WRAP_V1_0_CALL(getImsRegistrationStateResponse, info, isRegistered, ratFamily);
}

Return<void> RadioResponse::sendImsSmsResponse(const V1_0::RadioResponseInfo& info,
                                               const V1_0::SendSmsResult& sms) {
    WRAP_V1_0_CALL(sendImsSmsResponse, info, sms);
}

Return<void> RadioResponse::iccTransmitApduBasicChannelResponse(const V1_0::RadioResponseInfo& info,
                                                                const V1_0::IccIoResult& result) {
    WRAP_V1_0_CALL(iccTransmitApduBasicChannelResponse, info, result);
}

Return<void> RadioResponse::iccOpenLogicalChannelResponse(const V1_0::RadioResponseInfo& info,
                                                          int32_t channelId,
                                                          const hidl_vec<int8_t>& selectResponse) {
    WRAP_V1_0_CALL(iccOpenLogicalChannelResponse, info, channelId, selectResponse);
}

Return<void> RadioResponse::iccCloseLogicalChannelResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(iccCloseLogicalChannelResponse, info);
}

Return<void> RadioResponse::iccTransmitApduLogicalChannelResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::IccIoResult& result) {
    WRAP_V1_0_CALL(iccTransmitApduLogicalChannelResponse, info, result);
}

Return<void> RadioResponse::nvReadItemResponse(const V1_0::RadioResponseInfo& info,
                                               const hidl_string& result) {
    WRAP_V1_0_CALL(nvReadItemResponse, info, result);
}

Return<void> RadioResponse::nvWriteItemResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(nvWriteItemResponse, info);
}

Return<void> RadioResponse::nvWriteCdmaPrlResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(nvWriteCdmaPrlResponse, info);
}

Return<void> RadioResponse::nvResetConfigResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(nvResetConfigResponse, info);
}

Return<void> RadioResponse::setUiccSubscriptionResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setUiccSubscriptionResponse, info);
}

Return<void> RadioResponse::setDataAllowedResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setDataAllowedResponse, info);
}

Return<void> RadioResponse::getHardwareConfigResponse(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_0::HardwareConfig>& config) {
    WRAP_V1_0_CALL(getHardwareConfigResponse, info, config);
}

Return<void> RadioResponse::requestIccSimAuthenticationResponse(const V1_0::RadioResponseInfo& info,
                                                                const V1_0::IccIoResult& result) {
    WRAP_V1_0_CALL(requestIccSimAuthenticationResponse, info, result);
}

Return<void> RadioResponse::setDataProfileResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setDataProfileResponse, info);
}

Return<void> RadioResponse::requestShutdownResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(requestShutdownResponse, info);
}

Return<void> RadioResponse::getRadioCapabilityResponse(const V1_0::RadioResponseInfo& info,
                                                       const V1_0::RadioCapability& rc) {
    WRAP_V1_0_CALL(getRadioCapabilityResponse, info, rc);
}

Return<void> RadioResponse::setRadioCapabilityResponse(const V1_0::RadioResponseInfo& info,
                                                       const V1_0::RadioCapability& rc) {
    WRAP_V1_0_CALL(setRadioCapabilityResponse, info, rc);
}

Return<void> RadioResponse::startLceServiceResponse(const V1_0::RadioResponseInfo& info,
                                                    const V1_0::LceStatusInfo& statusInfo) {
    WRAP_V1_0_CALL(startLceServiceResponse, info, statusInfo);
}

Return<void> RadioResponse::stopLceServiceResponse(const V1_0::RadioResponseInfo& info,
                                                   const V1_0::LceStatusInfo& statusInfo) {
    WRAP_V1_0_CALL(stopLceServiceResponse, info, statusInfo);
}

Return<void> RadioResponse::pullLceDataResponse(const V1_0::RadioResponseInfo& info,
                                                const V1_0::LceDataInfo& lceInfo) {
    WRAP_V1_0_CALL(pullLceDataResponse, info, lceInfo);
}

Return<void> RadioResponse::getModemActivityInfoResponse(
        const V1_0::RadioResponseInfo& info, const V1_0::ActivityStatsInfo& activityInfo) {
    WRAP_V1_0_CALL(getModemActivityInfoResponse, info, activityInfo);
}

Return<void> RadioResponse::setAllowedCarriersResponse(const V1_0::RadioResponseInfo& info,
                                                       int32_t numAllowed) {
    MAYBE_WRAP_V1_4_CALL(setAllowedCarriersResponse_1_4, info);

    WRAP_V1_0_CALL(setAllowedCarriersResponse, info, numAllowed);
}

Return<void> RadioResponse::getAllowedCarriersResponse(const V1_0::RadioResponseInfo& info,
                                                       bool allAllowed,
                                                       const V1_0::CarrierRestrictions& carriers) {
    V1_4::CarrierRestrictionsWithPriority newCarriers = {};
    newCarriers.allowedCarriersPrioritized = !allAllowed;
    if (!allAllowed) {
        newCarriers.allowedCarriers = carriers.allowedCarriers;
        newCarriers.excludedCarriers = carriers.excludedCarriers;
    }
    MAYBE_WRAP_V1_4_CALL(getAllowedCarriersResponse_1_4, info, newCarriers,
                         V1_4::SimLockMultiSimPolicy::NO_MULTISIM_POLICY);

    WRAP_V1_0_CALL(getAllowedCarriersResponse, info, allAllowed, carriers);
}

Return<void> RadioResponse::sendDeviceStateResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(sendDeviceStateResponse, info);
}

Return<void> RadioResponse::setIndicationFilterResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setIndicationFilterResponse, info);
}

Return<void> RadioResponse::setSimCardPowerResponse(const V1_0::RadioResponseInfo& info) {
    WRAP_V1_0_CALL(setSimCardPowerResponse, info);
}

Return<void> RadioResponse::acknowledgeRequest(int32_t serial) {
    WRAP_V1_0_CALL(acknowledgeRequest, serial);
}

// Methods from ::android::hardware::radio::V1_1::IRadioResponse follow.
Return<void> RadioResponse::setCarrierInfoForImsiEncryptionResponse(
        const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_1_CALL(setCarrierInfoForImsiEncryptionResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSimCardPowerResponse_1_1(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_1_CALL(setSimCardPowerResponse_1_1, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::startNetworkScanResponse(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_4_CALL(startNetworkScanResponse_1_4, info);

    MAYBE_WRAP_V1_1_CALL(startNetworkScanResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopNetworkScanResponse(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_1_CALL(stopNetworkScanResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::startKeepaliveResponse(const V1_0::RadioResponseInfo& info,
                                                   const V1_1::KeepaliveStatus& status) {
    MAYBE_WRAP_V1_1_CALL(startKeepaliveResponse, info, status);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::stopKeepaliveResponse(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_1_CALL(stopKeepaliveResponse, info);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadioResponse follow.
Return<void> RadioResponse::getCellInfoListResponse_1_2(const V1_0::RadioResponseInfo& info,
                                                        const hidl_vec<V1_2::CellInfo>& cellInfo) {
    MAYBE_WRAP_V1_4_CALL(getCellInfoListResponse_1_4, info, Create1_4CellInfoList(cellInfo));

    MAYBE_WRAP_V1_2_CALL(getCellInfoListResponse_1_2, info, cellInfo);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIccCardStatusResponse_1_2(const V1_0::RadioResponseInfo& info,
                                                         const V1_2::CardStatus& cardStatus) {
    MAYBE_WRAP_V1_4_CALL(getIccCardStatusResponse_1_4, info, {cardStatus, hidl_string("")});

    MAYBE_WRAP_V1_2_CALL(getIccCardStatusResponse_1_2, info, cardStatus);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::setSignalStrengthReportingCriteriaResponse(
        const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_2_CALL(setSignalStrengthReportingCriteriaResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::setLinkCapacityReportingCriteriaResponse(
        const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_2_CALL(setLinkCapacityReportingCriteriaResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCurrentCallsResponse_1_2(const V1_0::RadioResponseInfo& info,
                                                        const hidl_vec<V1_2::Call>& calls) {
    MAYBE_WRAP_V1_2_CALL(getCurrentCallsResponse_1_2, info, calls);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse_1_2(
        const V1_0::RadioResponseInfo& info, const V1_2::SignalStrength& signalStrength) {
    MAYBE_WRAP_V1_4_CALL(getSignalStrengthResponse_1_4, info,
                         Create1_4SignalStrength(signalStrength));

    MAYBE_WRAP_V1_2_CALL(getSignalStrengthResponse_1_2, info, signalStrength);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getVoiceRegistrationStateResponse_1_2(
        const V1_0::RadioResponseInfo& info, const V1_2::VoiceRegStateResult& voiceRegResponse) {
    MAYBE_WRAP_V1_2_CALL(getVoiceRegistrationStateResponse_1_2, info, voiceRegResponse);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse_1_2(
        const V1_0::RadioResponseInfo& info, const V1_2::DataRegStateResult& dataRegResponse) {
    mDataRoaming = (dataRegResponse.regState == V1_0::RegState::REG_ROAMING);
    V1_4::DataRegStateResult newDRR = {};
    newDRR.base = dataRegResponse;
    MAYBE_WRAP_V1_4_CALL(getDataRegistrationStateResponse_1_4, info, newDRR);

    MAYBE_WRAP_V1_2_CALL(getDataRegistrationStateResponse_1_2, info, dataRegResponse);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_3::IRadioResponse follow.
Return<void> RadioResponse::setSystemSelectionChannelsResponse(
        const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_3_CALL(setSystemSelectionChannelsResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::enableModemResponse(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_3_CALL(enableModemResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getModemStackStatusResponse(const V1_0::RadioResponseInfo& info,
                                                        bool isEnabled) {
    MAYBE_WRAP_V1_3_CALL(getModemStackStatusResponse, info, isEnabled);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadioResponse follow.
Return<void> RadioResponse::emergencyDialResponse(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_4_CALL(emergencyDialResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::startNetworkScanResponse_1_4(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_4_CALL(startNetworkScanResponse_1_4, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getCellInfoListResponse_1_4(const V1_0::RadioResponseInfo& info,
                                                        const hidl_vec<V1_4::CellInfo>& cellInfo) {
    MAYBE_WRAP_V1_4_CALL(getCellInfoListResponse_1_4, info, cellInfo);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataRegistrationStateResponse_1_4(
        const V1_0::RadioResponseInfo& info, const V1_4::DataRegStateResult& dataRegResponse) {
    MAYBE_WRAP_V1_4_CALL(getDataRegistrationStateResponse_1_4, info, dataRegResponse);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getIccCardStatusResponse_1_4(const V1_0::RadioResponseInfo& info,
                                                         const V1_4::CardStatus& cardStatus) {
    MAYBE_WRAP_V1_4_CALL(getIccCardStatusResponse_1_4, info, cardStatus);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getPreferredNetworkTypeBitmapResponse(
        const V1_0::RadioResponseInfo& info,
        hidl_bitfield<V1_4::RadioAccessFamily> networkTypeBitmap) {
    MAYBE_WRAP_V1_4_CALL(getPreferredNetworkTypeBitmapResponse, info, networkTypeBitmap);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::setPreferredNetworkTypeBitmapResponse(
        const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_4_CALL(setPreferredNetworkTypeBitmapResponse, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getDataCallListResponse_1_4(
        const V1_0::RadioResponseInfo& info,
        const hidl_vec<V1_4::SetupDataCallResult>& dcResponse) {
    MAYBE_WRAP_V1_4_CALL(getDataCallListResponse_1_4, info, dcResponse);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::setupDataCallResponse_1_4(const V1_0::RadioResponseInfo& info,
                                                      const V1_4::SetupDataCallResult& dcResponse) {
    MAYBE_WRAP_V1_4_CALL(setupDataCallResponse_1_4, info, dcResponse);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::setAllowedCarriersResponse_1_4(const V1_0::RadioResponseInfo& info) {
    MAYBE_WRAP_V1_4_CALL(setAllowedCarriersResponse_1_4, info);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getAllowedCarriersResponse_1_4(
        const V1_0::RadioResponseInfo& info, const V1_4::CarrierRestrictionsWithPriority& carriers,
        V1_4::SimLockMultiSimPolicy multiSimPolicy) {
    MAYBE_WRAP_V1_4_CALL(getAllowedCarriersResponse_1_4, info, carriers, multiSimPolicy);

    // TODO implement
    return Void();
}

Return<void> RadioResponse::getSignalStrengthResponse_1_4(
        const V1_0::RadioResponseInfo& info, const V1_4::SignalStrength& signalStrength) {
    MAYBE_WRAP_V1_4_CALL(getSignalStrengthResponse_1_4, info, signalStrength);

    // TODO implement
    return Void();
}

void RadioResponse::setRealRadioResponse(sp<V1_0::IRadioResponse> realRadioResponse) {
    mRealRadioResponse = realRadioResponse;
}

bool RadioResponse::getDataRoaming() {
    return mDataRoaming;
}

V1_0::RadioTechnology RadioResponse::getRadioTechnology() {
    return mRadioTechnology;
}

sp<V1_1::IRadioResponse> RadioResponse::getRealRadioResponse_V1_1() {
    return V1_1::IRadioResponse::castFrom(mRealRadioResponse).withDefault(nullptr);
}

sp<V1_2::IRadioResponse> RadioResponse::getRealRadioResponse_V1_2() {
    return V1_2::IRadioResponse::castFrom(mRealRadioResponse).withDefault(nullptr);
}

sp<V1_3::IRadioResponse> RadioResponse::getRealRadioResponse_V1_3() {
    return V1_3::IRadioResponse::castFrom(mRealRadioResponse).withDefault(nullptr);
}

sp<V1_4::IRadioResponse> RadioResponse::getRealRadioResponse_V1_4() {
    return V1_4::IRadioResponse::castFrom(mRealRadioResponse).withDefault(nullptr);
}

}  // namespace android::hardware::radio::implementation
