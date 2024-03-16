/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.radio@1.4-service.legacy"

#include "Radio.h"

#include <android-base/logging.h>

#define WRAP_V1_0_CALL(method, ...)                                            \
    do {                                                                       \
        auto realRadio = mRealRadio;                                           \
        if (realRadio != nullptr) {                                            \
            return realRadio->method(__VA_ARGS__);                             \
        }                                                                      \
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE); \
    } while (0)

#define MAYBE_WRAP_V1_1_CALL(method, ...)               \
    do {                                                \
        auto realRadio_V1_1 = getRealRadio_V1_1();      \
        if (realRadio_V1_1 != nullptr) {                \
            return realRadio_V1_1->method(__VA_ARGS__); \
        }                                               \
    } while (0)

#define MAYBE_WRAP_V1_2_CALL(method, ...)               \
    do {                                                \
        auto realRadio_V1_2 = getRealRadio_V1_2();      \
        if (realRadio_V1_2 != nullptr) {                \
            return realRadio_V1_2->method(__VA_ARGS__); \
        }                                               \
    } while (0)

#define MAYBE_WRAP_V1_3_CALL(method, ...)               \
    do {                                                \
        auto realRadio_V1_3 = getRealRadio_V1_3();      \
        if (realRadio_V1_3 != nullptr) {                \
            return realRadio_V1_3->method(__VA_ARGS__); \
        }                                               \
    } while (0)

#define MAYBE_WRAP_V1_4_CALL(method, ...)                    \
    do {                                                     \
        auto realRadio_V1_4 = getRealRadio_V1_4();           \
        if (realRadio_V1_4 != nullptr) {                     \
            LOG(WARNING) << "Using wrapper when not needed"; \
            return realRadio_V1_4->method(__VA_ARGS__);      \
        }                                                    \
    } while (0)

namespace android::hardware::radio::implementation {

Radio::Radio(sp<::android::hardware::radio::V1_0::IRadio> realRadio) : mRealRadio(realRadio) {}

// Methods from ::android::hardware::radio::V1_0::IRadio follow.
Return<void> Radio::setResponseFunctions(
        const sp<::android::hardware::radio::V1_0::IRadioResponse>& radioResponse,
        const sp<::android::hardware::radio::V1_0::IRadioIndication>& radioIndication) {
    mRadioResponse = radioResponse;
    mRadioIndication = radioIndication;

    WRAP_V1_0_CALL(setResponseFunctions, radioResponse, radioIndication);
}

Return<void> Radio::getIccCardStatus(int32_t serial) {
    WRAP_V1_0_CALL(getIccCardStatus, serial);
}

Return<void> Radio::supplyIccPinForApp(int32_t serial, const hidl_string& pin,
                                       const hidl_string& aid) {
    WRAP_V1_0_CALL(supplyIccPinForApp, serial, pin, aid);
}

Return<void> Radio::supplyIccPukForApp(int32_t serial, const hidl_string& puk,
                                       const hidl_string& pin, const hidl_string& aid) {
    WRAP_V1_0_CALL(supplyIccPukForApp, serial, puk, pin, aid);
}

Return<void> Radio::supplyIccPin2ForApp(int32_t serial, const hidl_string& pin2,
                                        const hidl_string& aid) {
    WRAP_V1_0_CALL(supplyIccPin2ForApp, serial, pin2, aid);
}

Return<void> Radio::supplyIccPuk2ForApp(int32_t serial, const hidl_string& puk2,
                                        const hidl_string& pin2, const hidl_string& aid) {
    WRAP_V1_0_CALL(supplyIccPuk2ForApp, serial, puk2, pin2, aid);
}

Return<void> Radio::changeIccPinForApp(int32_t serial, const hidl_string& oldPin,
                                       const hidl_string& newPin, const hidl_string& aid) {
    WRAP_V1_0_CALL(changeIccPinForApp, serial, oldPin, newPin, aid);
}

Return<void> Radio::changeIccPin2ForApp(int32_t serial, const hidl_string& oldPin2,
                                        const hidl_string& newPin2, const hidl_string& aid) {
    WRAP_V1_0_CALL(changeIccPin2ForApp, serial, oldPin2, newPin2, aid);
}

Return<void> Radio::supplyNetworkDepersonalization(int32_t serial, const hidl_string& netPin) {
    WRAP_V1_0_CALL(supplyNetworkDepersonalization, serial, netPin);
}

Return<void> Radio::getCurrentCalls(int32_t serial) {
    WRAP_V1_0_CALL(getCurrentCalls, serial);
}

Return<void> Radio::dial(int32_t serial, const ::android::hardware::radio::V1_0::Dial& dialInfo) {
    WRAP_V1_0_CALL(dial, serial, dialInfo);
}

Return<void> Radio::getImsiForApp(int32_t serial, const hidl_string& aid) {
    WRAP_V1_0_CALL(getImsiForApp, serial, aid);
}

Return<void> Radio::hangup(int32_t serial, int32_t gsmIndex) {
    WRAP_V1_0_CALL(hangup, serial, gsmIndex);
}

Return<void> Radio::hangupWaitingOrBackground(int32_t serial) {
    WRAP_V1_0_CALL(hangupWaitingOrBackground, serial);
}

Return<void> Radio::hangupForegroundResumeBackground(int32_t serial) {
    WRAP_V1_0_CALL(hangupForegroundResumeBackground, serial);
}

Return<void> Radio::switchWaitingOrHoldingAndActive(int32_t serial) {
    WRAP_V1_0_CALL(switchWaitingOrHoldingAndActive, serial);
}

Return<void> Radio::conference(int32_t serial) {
    WRAP_V1_0_CALL(conference, serial);
}

Return<void> Radio::rejectCall(int32_t serial) {
    WRAP_V1_0_CALL(rejectCall, serial);
}

Return<void> Radio::getLastCallFailCause(int32_t serial) {
    WRAP_V1_0_CALL(getLastCallFailCause, serial);
}

Return<void> Radio::getSignalStrength(int32_t serial) {
    WRAP_V1_0_CALL(getSignalStrength, serial);
}

Return<void> Radio::getVoiceRegistrationState(int32_t serial) {
    WRAP_V1_0_CALL(getVoiceRegistrationState, serial);
}

Return<void> Radio::getDataRegistrationState(int32_t serial) {
    WRAP_V1_0_CALL(getDataRegistrationState, serial);
}

Return<void> Radio::getOperator(int32_t serial) {
    WRAP_V1_0_CALL(getOperator, serial);
}

Return<void> Radio::setRadioPower(int32_t serial, bool on) {
    WRAP_V1_0_CALL(setRadioPower, serial, on);
}

Return<void> Radio::sendDtmf(int32_t serial, const hidl_string& s) {
    WRAP_V1_0_CALL(sendDtmf, serial, s);
}

Return<void> Radio::sendSms(int32_t serial,
                            const ::android::hardware::radio::V1_0::GsmSmsMessage& message) {
    WRAP_V1_0_CALL(sendSms, serial, message);
}

Return<void> Radio::sendSMSExpectMore(
        int32_t serial, const ::android::hardware::radio::V1_0::GsmSmsMessage& message) {
    WRAP_V1_0_CALL(sendSMSExpectMore, serial, message);
}

Return<void> Radio::setupDataCall(
        int32_t serial, ::android::hardware::radio::V1_0::RadioTechnology radioTechnology,
        const ::android::hardware::radio::V1_0::DataProfileInfo& dataProfileInfo,
        bool modemCognitive, bool roamingAllowed, bool isRoaming) {
    WRAP_V1_0_CALL(setupDataCall, serial, radioTechnology, dataProfileInfo, modemCognitive,
                   roamingAllowed, isRoaming);
}

Return<void> Radio::iccIOForApp(int32_t serial,
                                const ::android::hardware::radio::V1_0::IccIo& iccIo) {
    WRAP_V1_0_CALL(iccIOForApp, serial, iccIo);
}

Return<void> Radio::sendUssd(int32_t serial, const hidl_string& ussd) {
    WRAP_V1_0_CALL(sendUssd, serial, ussd);
}

Return<void> Radio::cancelPendingUssd(int32_t serial) {
    WRAP_V1_0_CALL(cancelPendingUssd, serial);
}

Return<void> Radio::getClir(int32_t serial) {
    WRAP_V1_0_CALL(getClir, serial);
}

Return<void> Radio::setClir(int32_t serial, int32_t status) {
    WRAP_V1_0_CALL(setClir, serial, status);
}

Return<void> Radio::getCallForwardStatus(
        int32_t serial, const ::android::hardware::radio::V1_0::CallForwardInfo& callInfo) {
    WRAP_V1_0_CALL(getCallForwardStatus, serial, callInfo);
}

Return<void> Radio::setCallForward(
        int32_t serial, const ::android::hardware::radio::V1_0::CallForwardInfo& callInfo) {
    WRAP_V1_0_CALL(setCallForward, serial, callInfo);
}

Return<void> Radio::getCallWaiting(int32_t serial, int32_t serviceClass) {
    WRAP_V1_0_CALL(getCallWaiting, serial, serviceClass);
}

Return<void> Radio::setCallWaiting(int32_t serial, bool enable, int32_t serviceClass) {
    WRAP_V1_0_CALL(setCallWaiting, serial, enable, serviceClass);
}

Return<void> Radio::acknowledgeLastIncomingGsmSms(
        int32_t serial, bool success,
        ::android::hardware::radio::V1_0::SmsAcknowledgeFailCause cause) {
    WRAP_V1_0_CALL(acknowledgeLastIncomingGsmSms, serial, success, cause);
}

Return<void> Radio::acceptCall(int32_t serial) {
    WRAP_V1_0_CALL(acceptCall, serial);
}

Return<void> Radio::deactivateDataCall(int32_t serial, int32_t cid, bool reasonRadioShutDown) {
    WRAP_V1_0_CALL(deactivateDataCall, serial, cid, reasonRadioShutDown);
}

Return<void> Radio::getFacilityLockForApp(int32_t serial, const hidl_string& facility,
                                          const hidl_string& password, int32_t serviceClass,
                                          const hidl_string& appId) {
    WRAP_V1_0_CALL(getFacilityLockForApp, serial, facility, password, serviceClass, appId);
}

Return<void> Radio::setFacilityLockForApp(int32_t serial, const hidl_string& facility,
                                          bool lockState, const hidl_string& password,
                                          int32_t serviceClass, const hidl_string& appId) {
    WRAP_V1_0_CALL(setFacilityLockForApp, serial, facility, lockState, password, serviceClass,
                   appId);
}

Return<void> Radio::setBarringPassword(int32_t serial, const hidl_string& facility,
                                       const hidl_string& oldPassword,
                                       const hidl_string& newPassword) {
    WRAP_V1_0_CALL(setBarringPassword, serial, facility, oldPassword, newPassword);
}

Return<void> Radio::getNetworkSelectionMode(int32_t serial) {
    WRAP_V1_0_CALL(getNetworkSelectionMode, serial);
}

Return<void> Radio::setNetworkSelectionModeAutomatic(int32_t serial) {
    WRAP_V1_0_CALL(setNetworkSelectionModeAutomatic, serial);
}

Return<void> Radio::setNetworkSelectionModeManual(int32_t serial,
                                                  const hidl_string& operatorNumeric) {
    WRAP_V1_0_CALL(setNetworkSelectionModeManual, serial, operatorNumeric);
}

Return<void> Radio::getAvailableNetworks(int32_t serial) {
    WRAP_V1_0_CALL(getAvailableNetworks, serial);
}

Return<void> Radio::startDtmf(int32_t serial, const hidl_string& s) {
    WRAP_V1_0_CALL(startDtmf, serial, s);
}

Return<void> Radio::stopDtmf(int32_t serial) {
    WRAP_V1_0_CALL(stopDtmf, serial);
}

Return<void> Radio::getBasebandVersion(int32_t serial) {
    WRAP_V1_0_CALL(getBasebandVersion, serial);
}

Return<void> Radio::separateConnection(int32_t serial, int32_t gsmIndex) {
    WRAP_V1_0_CALL(separateConnection, serial, gsmIndex);
}

Return<void> Radio::setMute(int32_t serial, bool enable) {
    WRAP_V1_0_CALL(setMute, serial, enable);
}

Return<void> Radio::getMute(int32_t serial) {
    WRAP_V1_0_CALL(getMute, serial);
}

Return<void> Radio::getClip(int32_t serial) {
    WRAP_V1_0_CALL(getClip, serial);
}

Return<void> Radio::getDataCallList(int32_t serial) {
    WRAP_V1_0_CALL(getDataCallList, serial);
}

Return<void> Radio::setSuppServiceNotifications(int32_t serial, bool enable) {
    WRAP_V1_0_CALL(setSuppServiceNotifications, serial, enable);
}

Return<void> Radio::writeSmsToSim(
        int32_t serial, const ::android::hardware::radio::V1_0::SmsWriteArgs& smsWriteArgs) {
    WRAP_V1_0_CALL(writeSmsToSim, serial, smsWriteArgs);
}

Return<void> Radio::deleteSmsOnSim(int32_t serial, int32_t index) {
    WRAP_V1_0_CALL(deleteSmsOnSim, serial, index);
}

Return<void> Radio::setBandMode(int32_t serial,
                                ::android::hardware::radio::V1_0::RadioBandMode mode) {
    WRAP_V1_0_CALL(setBandMode, serial, mode);
}

Return<void> Radio::getAvailableBandModes(int32_t serial) {
    WRAP_V1_0_CALL(getAvailableBandModes, serial);
}

Return<void> Radio::sendEnvelope(int32_t serial, const hidl_string& command) {
    WRAP_V1_0_CALL(sendEnvelope, serial, command);
}

Return<void> Radio::sendTerminalResponseToSim(int32_t serial, const hidl_string& commandResponse) {
    WRAP_V1_0_CALL(sendTerminalResponseToSim, serial, commandResponse);
}

Return<void> Radio::handleStkCallSetupRequestFromSim(int32_t serial, bool accept) {
    WRAP_V1_0_CALL(handleStkCallSetupRequestFromSim, serial, accept);
}

Return<void> Radio::explicitCallTransfer(int32_t serial) {
    WRAP_V1_0_CALL(explicitCallTransfer, serial);
}

Return<void> Radio::setPreferredNetworkType(
        int32_t serial, ::android::hardware::radio::V1_0::PreferredNetworkType nwType) {
    WRAP_V1_0_CALL(setPreferredNetworkType, serial, nwType);
}

Return<void> Radio::getPreferredNetworkType(int32_t serial) {
    WRAP_V1_0_CALL(getPreferredNetworkType, serial);
}

Return<void> Radio::getNeighboringCids(int32_t serial) {
    WRAP_V1_0_CALL(getNeighboringCids, serial);
}

Return<void> Radio::setLocationUpdates(int32_t serial, bool enable) {
    WRAP_V1_0_CALL(setLocationUpdates, serial, enable);
}

Return<void> Radio::setCdmaSubscriptionSource(
        int32_t serial, ::android::hardware::radio::V1_0::CdmaSubscriptionSource cdmaSub) {
    WRAP_V1_0_CALL(setCdmaSubscriptionSource, serial, cdmaSub);
}

Return<void> Radio::setCdmaRoamingPreference(
        int32_t serial, ::android::hardware::radio::V1_0::CdmaRoamingType type) {
    WRAP_V1_0_CALL(setCdmaRoamingPreference, serial, type);
}

Return<void> Radio::getCdmaRoamingPreference(int32_t serial) {
    WRAP_V1_0_CALL(getCdmaRoamingPreference, serial);
}

Return<void> Radio::setTTYMode(int32_t serial, ::android::hardware::radio::V1_0::TtyMode mode) {
    WRAP_V1_0_CALL(setTTYMode, serial, mode);
}

Return<void> Radio::getTTYMode(int32_t serial) {
    WRAP_V1_0_CALL(getTTYMode, serial);
}

Return<void> Radio::setPreferredVoicePrivacy(int32_t serial, bool enable) {
    WRAP_V1_0_CALL(setPreferredVoicePrivacy, serial, enable);
}

Return<void> Radio::getPreferredVoicePrivacy(int32_t serial) {
    WRAP_V1_0_CALL(getPreferredVoicePrivacy, serial);
}

Return<void> Radio::sendCDMAFeatureCode(int32_t serial, const hidl_string& featureCode) {
    WRAP_V1_0_CALL(sendCDMAFeatureCode, serial, featureCode);
}

Return<void> Radio::sendBurstDtmf(int32_t serial, const hidl_string& dtmf, int32_t on,
                                  int32_t off) {
    WRAP_V1_0_CALL(sendBurstDtmf, serial, dtmf, on, off);
}

Return<void> Radio::sendCdmaSms(int32_t serial,
                                const ::android::hardware::radio::V1_0::CdmaSmsMessage& sms) {
    WRAP_V1_0_CALL(sendCdmaSms, serial, sms);
}

Return<void> Radio::acknowledgeLastIncomingCdmaSms(
        int32_t serial, const ::android::hardware::radio::V1_0::CdmaSmsAck& smsAck) {
    WRAP_V1_0_CALL(acknowledgeLastIncomingCdmaSms, serial, smsAck);
}

Return<void> Radio::getGsmBroadcastConfig(int32_t serial) {
    WRAP_V1_0_CALL(getGsmBroadcastConfig, serial);
}

Return<void> Radio::setGsmBroadcastConfig(
        int32_t serial,
        const hidl_vec<::android::hardware::radio::V1_0::GsmBroadcastSmsConfigInfo>& configInfo) {
    WRAP_V1_0_CALL(setGsmBroadcastConfig, serial, configInfo);
}

Return<void> Radio::setGsmBroadcastActivation(int32_t serial, bool activate) {
    WRAP_V1_0_CALL(setGsmBroadcastActivation, serial, activate);
}

Return<void> Radio::getCdmaBroadcastConfig(int32_t serial) {
    WRAP_V1_0_CALL(getCdmaBroadcastConfig, serial);
}

Return<void> Radio::setCdmaBroadcastConfig(
        int32_t serial,
        const hidl_vec<::android::hardware::radio::V1_0::CdmaBroadcastSmsConfigInfo>& configInfo) {
    WRAP_V1_0_CALL(setCdmaBroadcastConfig, serial, configInfo);
}

Return<void> Radio::setCdmaBroadcastActivation(int32_t serial, bool activate) {
    WRAP_V1_0_CALL(setCdmaBroadcastActivation, serial, activate);
}

Return<void> Radio::getCDMASubscription(int32_t serial) {
    WRAP_V1_0_CALL(getCDMASubscription, serial);
}

Return<void> Radio::writeSmsToRuim(
        int32_t serial, const ::android::hardware::radio::V1_0::CdmaSmsWriteArgs& cdmaSms) {
    WRAP_V1_0_CALL(writeSmsToRuim, serial, cdmaSms);
}

Return<void> Radio::deleteSmsOnRuim(int32_t serial, int32_t index) {
    WRAP_V1_0_CALL(deleteSmsOnRuim, serial, index);
}

Return<void> Radio::getDeviceIdentity(int32_t serial) {
    WRAP_V1_0_CALL(getDeviceIdentity, serial);
}

Return<void> Radio::exitEmergencyCallbackMode(int32_t serial) {
    WRAP_V1_0_CALL(exitEmergencyCallbackMode, serial);
}

Return<void> Radio::getSmscAddress(int32_t serial) {
    WRAP_V1_0_CALL(getSmscAddress, serial);
}

Return<void> Radio::setSmscAddress(int32_t serial, const hidl_string& smsc) {
    WRAP_V1_0_CALL(setSmscAddress, serial, smsc);
}

Return<void> Radio::reportSmsMemoryStatus(int32_t serial, bool available) {
    WRAP_V1_0_CALL(reportSmsMemoryStatus, serial, available);
}

Return<void> Radio::reportStkServiceIsRunning(int32_t serial) {
    WRAP_V1_0_CALL(reportStkServiceIsRunning, serial);
}

Return<void> Radio::getCdmaSubscriptionSource(int32_t serial) {
    WRAP_V1_0_CALL(getCdmaSubscriptionSource, serial);
}

Return<void> Radio::requestIsimAuthentication(int32_t serial, const hidl_string& challenge) {
    WRAP_V1_0_CALL(requestIsimAuthentication, serial, challenge);
}

Return<void> Radio::acknowledgeIncomingGsmSmsWithPdu(int32_t serial, bool success,
                                                     const hidl_string& ackPdu) {
    WRAP_V1_0_CALL(acknowledgeIncomingGsmSmsWithPdu, serial, success, ackPdu);
}

Return<void> Radio::sendEnvelopeWithStatus(int32_t serial, const hidl_string& contents) {
    WRAP_V1_0_CALL(sendEnvelopeWithStatus, serial, contents);
}

Return<void> Radio::getVoiceRadioTechnology(int32_t serial) {
    WRAP_V1_0_CALL(getVoiceRadioTechnology, serial);
}

Return<void> Radio::getCellInfoList(int32_t serial) {
    WRAP_V1_0_CALL(getCellInfoList, serial);
}

Return<void> Radio::setCellInfoListRate(int32_t serial, int32_t rate) {
    WRAP_V1_0_CALL(setCellInfoListRate, serial, rate);
}

Return<void> Radio::setInitialAttachApn(
        int32_t serial, const ::android::hardware::radio::V1_0::DataProfileInfo& dataProfileInfo,
        bool modemCognitive, bool isRoaming) {
    WRAP_V1_0_CALL(setInitialAttachApn, serial, dataProfileInfo, modemCognitive, isRoaming);
}

Return<void> Radio::getImsRegistrationState(int32_t serial) {
    WRAP_V1_0_CALL(getImsRegistrationState, serial);
}

Return<void> Radio::sendImsSms(int32_t serial,
                               const ::android::hardware::radio::V1_0::ImsSmsMessage& message) {
    WRAP_V1_0_CALL(sendImsSms, serial, message);
}

Return<void> Radio::iccTransmitApduBasicChannel(
        int32_t serial, const ::android::hardware::radio::V1_0::SimApdu& message) {
    WRAP_V1_0_CALL(iccTransmitApduBasicChannel, serial, message);
}

Return<void> Radio::iccOpenLogicalChannel(int32_t serial, const hidl_string& aid, int32_t p2) {
    WRAP_V1_0_CALL(iccOpenLogicalChannel, serial, aid, p2);
}

Return<void> Radio::iccCloseLogicalChannel(int32_t serial, int32_t channelId) {
    WRAP_V1_0_CALL(iccCloseLogicalChannel, serial, channelId);
}

Return<void> Radio::iccTransmitApduLogicalChannel(
        int32_t serial, const ::android::hardware::radio::V1_0::SimApdu& message) {
    WRAP_V1_0_CALL(iccTransmitApduLogicalChannel, serial, message);
}

Return<void> Radio::nvReadItem(int32_t serial, ::android::hardware::radio::V1_0::NvItem itemId) {
    WRAP_V1_0_CALL(nvReadItem, serial, itemId);
}

Return<void> Radio::nvWriteItem(int32_t serial,
                                const ::android::hardware::radio::V1_0::NvWriteItem& item) {
    WRAP_V1_0_CALL(nvWriteItem, serial, item);
}

Return<void> Radio::nvWriteCdmaPrl(int32_t serial, const hidl_vec<uint8_t>& prl) {
    WRAP_V1_0_CALL(nvWriteCdmaPrl, serial, prl);
}

Return<void> Radio::nvResetConfig(int32_t serial,
                                  ::android::hardware::radio::V1_0::ResetNvType resetType) {
    WRAP_V1_0_CALL(nvResetConfig, serial, resetType);
}

Return<void> Radio::setUiccSubscription(
        int32_t serial, const ::android::hardware::radio::V1_0::SelectUiccSub& uiccSub) {
    WRAP_V1_0_CALL(setUiccSubscription, serial, uiccSub);
}

Return<void> Radio::setDataAllowed(int32_t serial, bool allow) {
    WRAP_V1_0_CALL(setDataAllowed, serial, allow);
}

Return<void> Radio::getHardwareConfig(int32_t serial) {
    WRAP_V1_0_CALL(getHardwareConfig, serial);
}

Return<void> Radio::requestIccSimAuthentication(int32_t serial, int32_t authContext,
                                                const hidl_string& authData,
                                                const hidl_string& aid) {
    WRAP_V1_0_CALL(requestIccSimAuthentication, serial, authContext, authData, aid);
}

Return<void> Radio::setDataProfile(
        int32_t serial, const hidl_vec<::android::hardware::radio::V1_0::DataProfileInfo>& profiles,
        bool isRoaming) {
    WRAP_V1_0_CALL(setDataProfile, serial, profiles, isRoaming);
}

Return<void> Radio::requestShutdown(int32_t serial) {
    WRAP_V1_0_CALL(requestShutdown, serial);
}

Return<void> Radio::getRadioCapability(int32_t serial) {
    WRAP_V1_0_CALL(getRadioCapability, serial);
}

Return<void> Radio::setRadioCapability(
        int32_t serial, const ::android::hardware::radio::V1_0::RadioCapability& rc) {
    WRAP_V1_0_CALL(setRadioCapability, serial, rc);
}

Return<void> Radio::startLceService(int32_t serial, int32_t reportInterval, bool pullMode) {
    WRAP_V1_0_CALL(startLceService, serial, reportInterval, pullMode);
}

Return<void> Radio::stopLceService(int32_t serial) {
    WRAP_V1_0_CALL(stopLceService, serial);
}

Return<void> Radio::pullLceData(int32_t serial) {
    WRAP_V1_0_CALL(pullLceData, serial);
}

Return<void> Radio::getModemActivityInfo(int32_t serial) {
    WRAP_V1_0_CALL(getModemActivityInfo, serial);
}

Return<void> Radio::setAllowedCarriers(
        int32_t serial, bool allAllowed,
        const ::android::hardware::radio::V1_0::CarrierRestrictions& carriers) {
    WRAP_V1_0_CALL(setAllowedCarriers, serial, allAllowed, carriers);
}

Return<void> Radio::getAllowedCarriers(int32_t serial) {
    WRAP_V1_0_CALL(getAllowedCarriers, serial);
}

Return<void> Radio::sendDeviceState(
        int32_t serial, ::android::hardware::radio::V1_0::DeviceStateType deviceStateType,
        bool state) {
    WRAP_V1_0_CALL(sendDeviceState, serial, deviceStateType, state);
}

Return<void> Radio::setIndicationFilter(
        int32_t serial,
        hidl_bitfield<::android::hardware::radio::V1_0::IndicationFilter> indicationFilter) {
    WRAP_V1_0_CALL(setIndicationFilter, serial, indicationFilter);
}

Return<void> Radio::setSimCardPower(int32_t serial, bool powerUp) {
    WRAP_V1_0_CALL(setSimCardPower, serial, powerUp);
}

Return<void> Radio::responseAcknowledgement() {
    WRAP_V1_0_CALL(responseAcknowledgement);
}

// Methods from ::android::hardware::radio::V1_1::IRadio follow.
Return<void> Radio::setCarrierInfoForImsiEncryption(
        int32_t serial,
        const ::android::hardware::radio::V1_1::ImsiEncryptionInfo& imsiEncryptionInfo) {
    MAYBE_WRAP_V1_1_CALL(setCarrierInfoForImsiEncryption, serial, imsiEncryptionInfo);

    // TODO implement
    return Void();
}

Return<void> Radio::setSimCardPower_1_1(int32_t serial,
                                        ::android::hardware::radio::V1_1::CardPowerState powerUp) {
    MAYBE_WRAP_V1_1_CALL(setSimCardPower_1_1, serial, powerUp);

    // TODO implement
    return Void();
}

Return<void> Radio::startNetworkScan(
        int32_t serial, const ::android::hardware::radio::V1_1::NetworkScanRequest& request) {
    MAYBE_WRAP_V1_1_CALL(startNetworkScan, serial, request);

    // TODO implement
    return Void();
}

Return<void> Radio::stopNetworkScan(int32_t serial) {
    MAYBE_WRAP_V1_1_CALL(stopNetworkScan, serial);

    // TODO implement
    return Void();
}

Return<void> Radio::startKeepalive(
        int32_t serial, const ::android::hardware::radio::V1_1::KeepaliveRequest& keepalive) {
    MAYBE_WRAP_V1_1_CALL(startKeepalive, serial, keepalive);

    // TODO implement
    return Void();
}

Return<void> Radio::stopKeepalive(int32_t serial, int32_t sessionHandle) {
    MAYBE_WRAP_V1_1_CALL(stopKeepalive, serial, sessionHandle);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_2::IRadio follow.
Return<void> Radio::startNetworkScan_1_2(
        int32_t serial, const ::android::hardware::radio::V1_2::NetworkScanRequest& request) {
    MAYBE_WRAP_V1_2_CALL(startNetworkScan_1_2, serial, request);

    // TODO implement
    return Void();
}

Return<void> Radio::setIndicationFilter_1_2(
        int32_t serial,
        hidl_bitfield<::android::hardware::radio::V1_2::IndicationFilter> indicationFilter) {
    MAYBE_WRAP_V1_2_CALL(setIndicationFilter_1_2, serial, indicationFilter);

    // TODO implement
    return Void();
}

Return<void> Radio::setSignalStrengthReportingCriteria(
        int32_t serial, int32_t hysteresisMs, int32_t hysteresisDb,
        const hidl_vec<int32_t>& thresholdsDbm,
        ::android::hardware::radio::V1_2::AccessNetwork accessNetwork) {
    MAYBE_WRAP_V1_2_CALL(setSignalStrengthReportingCriteria, serial, hysteresisMs, hysteresisDb,
                         thresholdsDbm, accessNetwork);

    // TODO implement
    return Void();
}

Return<void> Radio::setLinkCapacityReportingCriteria(
        int32_t serial, int32_t hysteresisMs, int32_t hysteresisDlKbps, int32_t hysteresisUlKbps,
        const hidl_vec<int32_t>& thresholdsDownlinkKbps,
        const hidl_vec<int32_t>& thresholdsUplinkKbps,
        ::android::hardware::radio::V1_2::AccessNetwork accessNetwork) {
    MAYBE_WRAP_V1_2_CALL(setLinkCapacityReportingCriteria, serial, hysteresisMs, hysteresisDlKbps,
                         hysteresisUlKbps, thresholdsDownlinkKbps, thresholdsUplinkKbps,
                         accessNetwork);

    // TODO implement
    return Void();
}

Return<void> Radio::setupDataCall_1_2(
        int32_t serial, ::android::hardware::radio::V1_2::AccessNetwork accessNetwork,
        const ::android::hardware::radio::V1_0::DataProfileInfo& dataProfileInfo,
        bool modemCognitive, bool roamingAllowed, bool isRoaming,
        ::android::hardware::radio::V1_2::DataRequestReason reason,
        const hidl_vec<hidl_string>& addresses, const hidl_vec<hidl_string>& dnses) {
    MAYBE_WRAP_V1_2_CALL(setupDataCall_1_2, serial, accessNetwork, dataProfileInfo, modemCognitive,
                         roamingAllowed, isRoaming, reason, addresses, dnses);

    // TODO implement
    return Void();
}

Return<void> Radio::deactivateDataCall_1_2(
        int32_t serial, int32_t cid, ::android::hardware::radio::V1_2::DataRequestReason reason) {
    MAYBE_WRAP_V1_2_CALL(deactivateDataCall_1_2, serial, cid, reason);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_3::IRadio follow.
Return<void> Radio::setSystemSelectionChannels(
        int32_t serial, bool specifyChannels,
        const hidl_vec<::android::hardware::radio::V1_1::RadioAccessSpecifier>& specifiers) {
    MAYBE_WRAP_V1_3_CALL(setSystemSelectionChannels, serial, specifyChannels, specifiers);

    // TODO implement
    return Void();
}

Return<void> Radio::enableModem(int32_t serial, bool on) {
    MAYBE_WRAP_V1_3_CALL(enableModem, serial, on);

    // TODO implement
    return Void();
}

Return<void> Radio::getModemStackStatus(int32_t serial) {
    MAYBE_WRAP_V1_3_CALL(getModemStackStatus, serial);

    // TODO implement
    return Void();
}

// Methods from ::android::hardware::radio::V1_4::IRadio follow.
Return<void> Radio::setupDataCall_1_4(
        int32_t serial, ::android::hardware::radio::V1_4::AccessNetwork accessNetwork,
        const ::android::hardware::radio::V1_4::DataProfileInfo& dataProfileInfo,
        bool roamingAllowed, ::android::hardware::radio::V1_2::DataRequestReason reason,
        const hidl_vec<hidl_string>& addresses, const hidl_vec<hidl_string>& dnses) {
    MAYBE_WRAP_V1_4_CALL(setupDataCall_1_4, serial, accessNetwork, dataProfileInfo, roamingAllowed,
                         reason, addresses, dnses);

    // TODO implement
    return Void();
}

Return<void> Radio::setInitialAttachApn_1_4(
        int32_t serial, const ::android::hardware::radio::V1_4::DataProfileInfo& dataProfileInfo) {
    MAYBE_WRAP_V1_4_CALL(setInitialAttachApn_1_4, serial, dataProfileInfo);

    // TODO implement
    return Void();
}

Return<void> Radio::setDataProfile_1_4(
        int32_t serial,
        const hidl_vec<::android::hardware::radio::V1_4::DataProfileInfo>& profiles) {
    MAYBE_WRAP_V1_4_CALL(setDataProfile_1_4, serial, profiles);

    // TODO implement
    return Void();
}

Return<void> Radio::emergencyDial(
        int32_t serial, const ::android::hardware::radio::V1_0::Dial& dialInfo,
        hidl_bitfield<::android::hardware::radio::V1_4::EmergencyServiceCategory> categories,
        const hidl_vec<hidl_string>& urns,
        ::android::hardware::radio::V1_4::EmergencyCallRouting routing,
        bool hasKnownUserIntentEmergency, bool isTesting) {
    MAYBE_WRAP_V1_4_CALL(emergencyDial, serial, dialInfo, categories, urns, routing,
                         hasKnownUserIntentEmergency, isTesting);

    // TODO implement
    return Void();
}

Return<void> Radio::startNetworkScan_1_4(
        int32_t serial, const ::android::hardware::radio::V1_2::NetworkScanRequest& request) {
    MAYBE_WRAP_V1_4_CALL(startNetworkScan_1_4, serial, request);

    // TODO implement
    return Void();
}

Return<void> Radio::getPreferredNetworkTypeBitmap(int32_t serial) {
    MAYBE_WRAP_V1_4_CALL(getPreferredNetworkTypeBitmap, serial);

    // TODO implement
    return Void();
}

Return<void> Radio::setPreferredNetworkTypeBitmap(
        int32_t serial,
        hidl_bitfield<::android::hardware::radio::V1_4::RadioAccessFamily> networkTypeBitmap) {
    MAYBE_WRAP_V1_4_CALL(setPreferredNetworkTypeBitmap, serial, networkTypeBitmap);

    // TODO implement
    return Void();
}

Return<void> Radio::setAllowedCarriers_1_4(
        int32_t serial,
        const ::android::hardware::radio::V1_4::CarrierRestrictionsWithPriority& carriers,
        ::android::hardware::radio::V1_4::SimLockMultiSimPolicy multiSimPolicy) {
    MAYBE_WRAP_V1_4_CALL(setAllowedCarriers_1_4, serial, carriers, multiSimPolicy);

    // TODO implement
    return Void();
}

Return<void> Radio::getAllowedCarriers_1_4(int32_t serial) {
    MAYBE_WRAP_V1_4_CALL(getAllowedCarriers_1_4, serial);

    // TODO implement
    return Void();
}

Return<void> Radio::getSignalStrength_1_4(int32_t serial) {
    MAYBE_WRAP_V1_4_CALL(getSignalStrength_1_4, serial);

    // TODO implement
    return Void();
}

sp<::android::hardware::radio::V1_1::IRadio> Radio::getRealRadio_V1_1() {
    return ::android::hardware::radio::V1_1::IRadio::castFrom(mRealRadio).withDefault(nullptr);
}

sp<::android::hardware::radio::V1_2::IRadio> Radio::getRealRadio_V1_2() {
    return ::android::hardware::radio::V1_2::IRadio::castFrom(mRealRadio).withDefault(nullptr);
}

sp<::android::hardware::radio::V1_3::IRadio> Radio::getRealRadio_V1_3() {
    return ::android::hardware::radio::V1_3::IRadio::castFrom(mRealRadio).withDefault(nullptr);
}

sp<::android::hardware::radio::V1_4::IRadio> Radio::getRealRadio_V1_4() {
    return ::android::hardware::radio::V1_4::IRadio::castFrom(mRealRadio).withDefault(nullptr);
}

}  // namespace android::hardware::radio::implementation
