/*
 * Copyright (C) 2024 The LineageOS Project
 *               2024 Paranoid Android
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Legacy2Aidl.h"
#include "FingerprintEngineHidl.h"

namespace aidl::android::hardware::biometrics::fingerprint {

FingerprintEngineHidl::FingerprintEngineHidl(std::shared_ptr<FingerprintConfig> config)
    : FingerprintEngine(config) {
    if (mService) {
        ALOGI("fingerprint HAL already opened");
    } else {
        mService = IBiometricsFingerprint::getService();
        if (!mService) {
            ALOGE("Can't open any fingerprint HAL module");
        }
    }
}

std::shared_ptr<FingerprintEngine> FingerprintEngineHidl::create(
        std::shared_ptr<FingerprintConfig> config) {
    auto engine = std::make_shared<FingerprintEngineHidl>(std::move(config));
    if (engine->mService == nullptr) {
        ALOGE("Failed to set up fingerprint HIDL engine");
        return nullptr;
    }
    return engine;
}

void FingerprintEngineHidl::startSession(std::shared_ptr<ISessionCallback> cb, int userId) {
    mCb = std::move(cb);
    mUserId = userId;

    int ret = mService->setNotify(this);
    if (ret == 0) {
        ALOGE("Service can not notify callbacks");
        return;
    }

    auto path = std::format(FPDATA_PATH, userId);
    mService->setActiveGroup(userId, hidl_string(path));
}

void FingerprintEngineHidl::endSession() {
    mCb->onSessionClosed();

    int ret = mService->setNotify(nullptr);
    if (ret == 0) {
        ALOGE("Service can not notify callbacks");
    }
}

ndk::ScopedAStatus FingerprintEngineHidl::generateChallengeImpl() {
    uint64_t challenge = mService->preEnroll();
    ALOGI("generateChallenge: %ld", challenge);
    mCb->onChallengeGenerated(challenge);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::revokeChallengeImpl(int64_t challenge) {
    mService->postEnroll();
    mCb->onChallengeRevoked(challenge);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::enrollImpl(const HardwareAuthToken& hat) {
    hidl_array<uint8_t, 69> authToken;
    translate(hat, authToken);
    RequestStatus status = mService->enroll(authToken, mUserId, 60);
    if (status != RequestStatus::SYS_OK) {
        int error = RequestStatusToInt(status);
        mCb->onError(Error::UNABLE_TO_PROCESS, error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::authenticateImpl(int64_t operationId) {
    checkSensorLockout();
    RequestStatus status = mService->authenticate(operationId, mUserId);
    if (status != RequestStatus::SYS_OK) {
        int error = RequestStatusToInt(status);
        ALOGE("authenticate failed: %d", error);
        mCb->onError(Error::UNABLE_TO_PROCESS, error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::enumerateEnrollmentsImpl() {
    RequestStatus status =  mService->enumerate();
    if (status != RequestStatus::SYS_OK) {
        int error = RequestStatusToInt(status);
        ALOGE("enumerate failed: %d", error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::removeEnrollmentsImpl(const std::vector<int32_t>& enrollmentIds) {
    for (int32_t fid : enrollmentIds) {
        RequestStatus status = mService->remove(mUserId, fid);
        if (status != RequestStatus::SYS_OK) {
            ALOGE("remove failed: %d", RequestStatusToInt(status));
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::getAuthenticatorIdImpl() {
    uint64_t auth_id = mService->getAuthenticatorId();
    ALOGI("getAuthenticatorId: %ld", auth_id);
    mCb->onAuthenticatorIdRetrieved(auth_id);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::invalidateAuthenticatorIdImpl() {
    uint64_t auth_id = mService->getAuthenticatorId();
    ALOGI("invalidateAuthenticatorId: %ld", auth_id);
    mCb->onAuthenticatorIdInvalidated(auth_id);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngineHidl::cancelImpl() {
    RequestStatus status = mService->cancel();
    if (status != RequestStatus::SYS_OK) {
        int error = RequestStatusToInt(status);
        return ndk::ScopedAStatus::fromServiceSpecificError(error);
    }
    mCb->onError(Error::CANCELED, 0 /* vendorCode */);
    return ndk::ScopedAStatus::ok();
}

Return<void> FingerprintEngineHidl::onEnrollResult(uint64_t deviceId, uint32_t fingerId,
                                                   uint32_t groupId, uint32_t remaining) {
    ALOGD("onEnrollResult(fingerId=%d, groupId=%d, remaining=%d)",
          fingerId, groupId, remaining);
    mCb->onEnrollmentProgress(fingerId, remaining);
    return Void();
}

Return<void> FingerprintEngineHidl::onAcquired(uint64_t deviceId,
                                               FingerprintAcquiredInfo acquiredInfo,
                                               int32_t vendorCode) {
    ALOGD("onAcquired(%hhd, %d)", acquiredInfo, vendorCode);
    // don't process vendor messages further since frameworks try to disable
    // udfps display mode on vendor acquired messages but our sensors send a
    // vendor message during processing...
    AcquiredInfo info = VendorAcquiredFilter(acquiredInfo);
    if (info != AcquiredInfo::VENDOR) {
        mCb->onAcquired(info, vendorCode);
    }
    return Void();
}

Return<void> FingerprintEngineHidl::onAuthenticated(uint64_t deviceId, uint32_t fingerId,
                                                    uint32_t groupId, const hidl_vec<uint8_t>& token) {
    ALOGD("onAuthenticated(fingerId=%d, groupId=%d)", fingerId, groupId);
    if (fingerId != 0) {
        HardwareAuthToken authToken;
        translate(token, authToken);
        mCb->onAuthenticationSucceeded(fingerId, authToken);
        mLockoutTracker.reset(true);
    } else {
        mCb->onAuthenticationFailed();
        mLockoutTracker.addFailedAttempt();
        checkSensorLockout();
    }

    return Void();
}

Return<void> FingerprintEngineHidl::onError(uint64_t deviceId, FingerprintError error,
                                            int32_t vendorCode) {
    ALOGD("onError(%hhd, %d)", error, vendorCode);
    if (error != FingerprintError::ERROR_NO_ERROR) {
        Error errorAidl = VendorErrorFilter(error, &vendorCode);
        mCb->onError(errorAidl, vendorCode);
    }
    return Void();
}

Return<void> FingerprintEngineHidl::onRemoved(uint64_t deviceId, uint32_t fingerId,
                                              uint32_t groupId, uint32_t remaining) {
    ALOGD("onRemoved(fingerId=%d, groupId=%d, remaining=%d)",
          fingerId, groupId, remaining);
    std::vector<int> enrollments;
    enrollments.push_back(fingerId);
    mCb->onEnrollmentsRemoved(enrollments);
    return Void();
}

Return<void> FingerprintEngineHidl::onEnumerate(uint64_t deviceId, uint32_t fingerId,
                                               uint32_t groupId, uint32_t remaining) {
    ALOGD("onEnumerate(fingerId=%d, groupId=%d, remaining=%d)",
          fingerId, groupId, remaining);
    static std::vector<int> enrollments;
    enrollments.push_back(fingerId);
    if (remaining == 0) {
        mCb->onEnrollmentsEnumerated(enrollments);
        enrollments.clear();
    }
    return Void();
}

// Translate Error from HIDL to AIDL 
Error FingerprintEngineHidl::VendorErrorFilter(FingerprintError error, int32_t* vendorCode) {
    switch (error) {
        case FingerprintError::ERROR_HW_UNAVAILABLE:
            return Error::HW_UNAVAILABLE;
        case FingerprintError::ERROR_UNABLE_TO_PROCESS:
            return Error::UNABLE_TO_PROCESS;
        case FingerprintError::ERROR_TIMEOUT:
            return Error::TIMEOUT;
        case FingerprintError::ERROR_NO_SPACE:
            return Error::NO_SPACE;
        case FingerprintError::ERROR_CANCELED:
            return Error::CANCELED;
        case FingerprintError::ERROR_UNABLE_TO_REMOVE:
            return Error::UNABLE_TO_REMOVE;
        case FingerprintError::ERROR_LOCKOUT:
            *vendorCode = static_cast<int32_t>(error);
            return Error::VENDOR;
        case FingerprintError::ERROR_VENDOR:
            return Error::VENDOR;
        default:
            return Error::UNABLE_TO_PROCESS;
    }
}

// Translate AcquiredInfo from HIDL to AIDL 
AcquiredInfo FingerprintEngineHidl::VendorAcquiredFilter(FingerprintAcquiredInfo info) {
    switch (info) {
        case FingerprintAcquiredInfo::ACQUIRED_GOOD:
            return AcquiredInfo::GOOD;
        case FingerprintAcquiredInfo::ACQUIRED_PARTIAL:
            return AcquiredInfo::PARTIAL;
        case FingerprintAcquiredInfo::ACQUIRED_INSUFFICIENT:
            return AcquiredInfo::INSUFFICIENT;
        case FingerprintAcquiredInfo::ACQUIRED_IMAGER_DIRTY:
            return AcquiredInfo::SENSOR_DIRTY;
        case FingerprintAcquiredInfo::ACQUIRED_TOO_SLOW:
            return AcquiredInfo::TOO_SLOW;
        case FingerprintAcquiredInfo::ACQUIRED_TOO_FAST:
            return AcquiredInfo::TOO_FAST;
        case FingerprintAcquiredInfo::ACQUIRED_VENDOR:
            return AcquiredInfo::VENDOR;
        default:
            return AcquiredInfo::INSUFFICIENT;
    }
}

int32_t FingerprintEngineHidl::RequestStatusToInt(RequestStatus error) {
    switch (error) {
        case RequestStatus::SYS_OK:
            return 0;
        case RequestStatus::SYS_ENOENT:
            return -ENOENT;
        case RequestStatus::SYS_EINTR:
            return -EINTR;
        case RequestStatus::SYS_EIO:
            return -EIO;
        case RequestStatus::SYS_EAGAIN:
            return -EAGAIN;
        case RequestStatus::SYS_ENOMEM:
            return -ENOMEM;
        case RequestStatus::SYS_EACCES:
            return -EACCES;
        case RequestStatus::SYS_EFAULT:
            return -EFAULT;
        case RequestStatus::SYS_EBUSY:
            return -EBUSY;
        case RequestStatus::SYS_EINVAL:
            return -EINVAL;
        case RequestStatus::SYS_ENOSPC:
            return -ENOSPC;
        case RequestStatus::SYS_ETIMEDOUT:
            return -ETIMEDOUT;
        default:
            return -EINVAL;
    }
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
