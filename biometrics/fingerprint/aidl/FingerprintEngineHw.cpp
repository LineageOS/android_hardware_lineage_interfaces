/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/strings.h>

#include "Legacy2Aidl.h"
#include "FingerprintEngineHw.h"

namespace aidl::android::hardware::biometrics::fingerprint {

static FingerprintEngineHw* sInstance = nullptr;

FingerprintEngineHw::FingerprintEngineHw(std::shared_ptr<FingerprintConfig> config)
    : FingerprintEngine(config), mDevice(openHal()) {
    sInstance = this;
}

FingerprintEngineHw::~FingerprintEngineHw() {
    sInstance = nullptr;

    if (mDevice != nullptr) {
        int err;
        if (0 != (err = mDevice->common.close(reinterpret_cast<hw_device_t*>(mDevice)))) {
            ALOGE("Can't close fingerprint module, error: %d", err);
            return;
        }
        mDevice = nullptr;
    }
}

void FingerprintEngineHw::startSession(std::shared_ptr<ISessionCallback> cb, int userId) {
    mCb = std::move(cb);
    mUserId = userId;

    int err = mDevice->set_notify(mDevice, FingerprintEngineHw::notify);
    if (err != 0) {
        ALOGE("Can't register fingerprint module callback, error: %d", err);
        return;
    }

    auto path = std::format(FPDATA_PATH, userId);
    mDevice->set_active_group(mDevice, userId, path.c_str());
}

void FingerprintEngineHw::endSession() {
    mCb->onSessionClosed();

    int err = mDevice->set_notify(mDevice, nullptr);
    if (err != 0) {
        ALOGE("Can't register fingerprint module callback, error: %d", err);
    }
}

ndk::ScopedAStatus FingerprintEngineHw::generateChallengeImpl() {
    uint64_t challenge = mDevice->pre_enroll(mDevice);
    ALOGI("generateChallenge: %lu", challenge);
    mCb->onChallengeGenerated(challenge);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::revokeChallengeImpl(int64_t challenge) {
    mDevice->post_enroll(mDevice);
    mCb->onChallengeRevoked(challenge);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::enrollImpl(const HardwareAuthToken& hat) {
    hw_auth_token_t authToken;
    translate(hat, authToken);
    int error = mDevice->enroll(mDevice, &authToken, mUserId, 60);
    if (error) {
        ALOGE("enroll failed: %d", error);
        mCb->onError(Error::UNABLE_TO_PROCESS, error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::authenticateImpl(int64_t operationId) {
    checkSensorLockout();
    int error = mDevice->authenticate(mDevice, operationId, mUserId);
    if (error) {
        ALOGE("authenticate failed: %d", error);
        mCb->onError(Error::UNABLE_TO_PROCESS, error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::enumerateEnrollmentsImpl() {
    int error = mDevice->enumerate(mDevice);
    if (error) {
        ALOGE("enumerate failed: %d", error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::removeEnrollmentsImpl(const std::vector<int32_t>& enrollmentIds) {
    for (int32_t fid : enrollmentIds) {
        int error = mDevice->remove(mDevice, mUserId, fid);
        if (error) {
            ALOGE("remove failed: %d", error);
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::getAuthenticatorIdImpl() {
    uint64_t auth_id = mDevice->get_authenticator_id(mDevice);
    ALOGI("getAuthenticatorId: %lu", auth_id);
    mCb->onAuthenticatorIdRetrieved(auth_id);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::invalidateAuthenticatorIdImpl() {
    uint64_t auth_id = mDevice->get_authenticator_id(mDevice);
    ALOGI("invalidateAuthenticatorId: %lu", auth_id);
    mCb->onAuthenticatorIdInvalidated(auth_id);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHw::cancelImpl() {
    int ret = mDevice->cancel(mDevice);
    if (ret == 0) {
        mCb->onError(Error::CANCELED, 0 /* vendorCode */);
        return ndk::ScopedAStatus::ok();
    }
    return ndk::ScopedAStatus::fromServiceSpecificError(ret);
}

fingerprint_device_t* FingerprintEngineHw::openHal() {
    int err;
    const hw_module_t* hw_mdl = nullptr;
    ALOGD("Opening fingerprint hal library...");
    if (0 != (err = hw_get_module(FINGERPRINT_HARDWARE_MODULE_ID, &hw_mdl))) {
        ALOGE("Can't open fingerprint HW Module, error: %d", err);
        return nullptr;
    }

    if (hw_mdl == nullptr) {
        ALOGE("No valid fingerprint module");
        return nullptr;
    }

    fingerprint_module_t const* module = reinterpret_cast<const fingerprint_module_t*>(hw_mdl);
    if (module->common.methods->open == nullptr) {
        ALOGE("No valid open method");
        return nullptr;
    }

    hw_device_t* device = nullptr;

    if (0 != (err = module->common.methods->open(hw_mdl, nullptr, &device))) {
        ALOGE("Can't open fingerprint methods, error: %d", err);
        return nullptr;
    }

    if (kVersion != device->version) {
        // enforce version on new devices because of HIDL@2.1 translation layer
        ALOGE("Wrong fp version. Expected %d, got %d", kVersion, device->version);
        return nullptr;
    }

    return reinterpret_cast<fingerprint_device_t*>(device);
}

void FingerprintEngineHw::notify(const fingerprint_msg_t* msg) {
    if (sInstance == nullptr || sInstance->mCb == nullptr) {
        ALOGE("Can't dispatch notification: Engine context or Callback is missing");
        return;
    }

    switch (msg->type) {
        case FINGERPRINT_ERROR: {
            int32_t vendorCode = 0;
            Error result = sInstance->VendorErrorFilter(msg->data.error, &vendorCode);
            ALOGD("onError(%d, %d)", static_cast<int>(result), vendorCode);
            sInstance->mCb->onError(result, vendorCode);
        } break;
        case FINGERPRINT_ACQUIRED: {
            int32_t vendorCode = 0;
            AcquiredInfo result =
                    VendorAcquiredFilter(msg->data.acquired.acquired_info, &vendorCode);
            ALOGD("onAcquired(%hhd, %d)", result, vendorCode);
            // don't process vendor messages further since frameworks try to disable
            // udfps display mode on vendor acquired messages but our sensors send a
            // vendor message during processing...
            if (result != AcquiredInfo::VENDOR) {
                sInstance->mCb->onAcquired(result, vendorCode);
            }
        } break;
        case FINGERPRINT_TEMPLATE_ENROLLING: {
            ALOGD("onEnrollResult(fid=%d, gid=%d, rem=%d)", msg->data.enroll.finger.fid,
                  msg->data.enroll.finger.gid, msg->data.enroll.samples_remaining);
            sInstance->mCb->onEnrollmentProgress(msg->data.enroll.finger.fid,
                                      msg->data.enroll.samples_remaining);
        } break;
        case FINGERPRINT_TEMPLATE_REMOVED: {
            ALOGD("onRemove(fid=%d, gid=%d, rem=%d)", msg->data.removed.finger.fid,
                  msg->data.removed.finger.gid, msg->data.removed.remaining_templates);
            std::vector<int32_t> enrollments;
            enrollments.push_back(msg->data.removed.finger.fid);
            sInstance->mCb->onEnrollmentsRemoved(enrollments);
        } break;
        case FINGERPRINT_AUTHENTICATED: {
            ALOGD("onAuthenticated(fid=%d, gid=%d)", msg->data.authenticated.finger.fid,
                  msg->data.authenticated.finger.gid);
            if (msg->data.authenticated.finger.fid != 0) {
                const hw_auth_token_t hat = msg->data.authenticated.hat;
                HardwareAuthToken authToken;
                translate(hat, authToken);

                sInstance->mCb->onAuthenticationSucceeded(msg->data.authenticated.finger.fid, authToken);
                sInstance->clearLockout(true);
            } else {
                sInstance->mCb->onAuthenticationFailed();
                sInstance->checkSensorLockout();
            }
        } break;
        case FINGERPRINT_TEMPLATE_ENUMERATING: {
            ALOGD("onEnumerate(fid=%d, gid=%d, rem=%d)", msg->data.enumerated.finger.fid,
                  msg->data.enumerated.finger.gid, msg->data.enumerated.remaining_templates);
            static std::vector<int32_t> enrollments;
            enrollments.push_back(msg->data.enumerated.finger.fid);
            if (msg->data.enumerated.remaining_templates == 0) {
                sInstance->mCb->onEnrollmentsEnumerated(enrollments);
                enrollments.clear();
            }
        } break;
    }
}

Error FingerprintEngineHw::VendorErrorFilter(int32_t error, int32_t* vendorCode) {
    *vendorCode = 0;
    switch (error) {
        case FINGERPRINT_ERROR_HW_UNAVAILABLE:
            return Error::HW_UNAVAILABLE;
        case FINGERPRINT_ERROR_UNABLE_TO_PROCESS:
            return Error::UNABLE_TO_PROCESS;
        case FINGERPRINT_ERROR_TIMEOUT:
            return Error::TIMEOUT;
        case FINGERPRINT_ERROR_NO_SPACE:
            return Error::NO_SPACE;
        case FINGERPRINT_ERROR_CANCELED:
            return Error::CANCELED;
        case FINGERPRINT_ERROR_UNABLE_TO_REMOVE:
            return Error::UNABLE_TO_REMOVE;
        case FINGERPRINT_ERROR_LOCKOUT: {
            *vendorCode = FINGERPRINT_ERROR_LOCKOUT;
            return Error::VENDOR;
        }
        default:
            if (error >= FINGERPRINT_ERROR_VENDOR_BASE) {
                *vendorCode = error - FINGERPRINT_ERROR_VENDOR_BASE;
                return Error::VENDOR;
            }
    }
    ALOGE("Unknown error from fingerprint vendor library: %d", error);
    return Error::UNABLE_TO_PROCESS;
}

AcquiredInfo FingerprintEngineHw::VendorAcquiredFilter(int32_t info, int32_t* vendorCode) {
    *vendorCode = 0;
    switch (info) {
        case FINGERPRINT_ACQUIRED_GOOD:
            return AcquiredInfo::GOOD;
        case FINGERPRINT_ACQUIRED_PARTIAL:
            return AcquiredInfo::PARTIAL;
        case FINGERPRINT_ACQUIRED_INSUFFICIENT:
            return AcquiredInfo::INSUFFICIENT;
        case FINGERPRINT_ACQUIRED_IMAGER_DIRTY:
            return AcquiredInfo::SENSOR_DIRTY;
        case FINGERPRINT_ACQUIRED_TOO_SLOW:
            return AcquiredInfo::TOO_SLOW;
        case FINGERPRINT_ACQUIRED_TOO_FAST:
            return AcquiredInfo::TOO_FAST;
        default:
            if (info >= FINGERPRINT_ACQUIRED_VENDOR_BASE) {
                *vendorCode = info - FINGERPRINT_ACQUIRED_VENDOR_BASE;
                return AcquiredInfo::VENDOR;
            }
    }
    ALOGE("Unknown acquired message from fingerprint vendor library: %d", info);
    return AcquiredInfo::UNKNOWN;
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
