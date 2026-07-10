/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/fingerprint/ISessionCallback.h>
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprint.h>
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprintClientCallback.h>
#include <android/hardware/biometrics/fingerprint/2.1/types.h>
#include <android/log.h>
#include <hardware/hardware.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <log/log.h>

#include "FingerprintEngine.h"

using ::aidl::android::hardware::keymaster::HardwareAuthToken;
using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_array;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::biometrics::fingerprint::V2_1::FingerprintError;
using ::android::hardware::biometrics::fingerprint::V2_1::FingerprintAcquiredInfo;
using ::android::hardware::biometrics::fingerprint::V2_1::RequestStatus;
using ::android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprint;
using ::android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprintClientCallback;

namespace aidl::android::hardware::biometrics::fingerprint {

class FingerprintEngineHidl : public FingerprintEngine,
                              public IBiometricsFingerprintClientCallback {
  public:
    FingerprintEngineHidl(std::shared_ptr<FingerprintConfig> config);

    void startSession(std::shared_ptr<ISessionCallback> cb, int userId) override;
    void endSession() override;
    ndk::ScopedAStatus generateChallengeImpl() override;
    ndk::ScopedAStatus revokeChallengeImpl(int64_t challenge) override;
    ndk::ScopedAStatus enrollImpl(const HardwareAuthToken& hat) override;
    ndk::ScopedAStatus authenticateImpl(int64_t operationId) override;
    ndk::ScopedAStatus enumerateEnrollmentsImpl() override;
    ndk::ScopedAStatus removeEnrollmentsImpl(const std::vector<int32_t>& enrollmentIds) override;
    ndk::ScopedAStatus getAuthenticatorIdImpl() override;
    ndk::ScopedAStatus invalidateAuthenticatorIdImpl() override;
    ndk::ScopedAStatus cancelImpl() override;

    // Methods from ::android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprintClientCallback follow.
    Return<void> onEnrollResult(uint64_t deviceId, uint32_t fingerId, uint32_t groupId,
                                uint32_t remaining) override;
    Return<void> onAcquired(uint64_t deviceId, FingerprintAcquiredInfo acquiredInfo,
                            int32_t vendorCode) override;
    Return<void> onAuthenticated(uint64_t deviceId, uint32_t fingerId, uint32_t groupId,
                                 const hidl_vec<uint8_t>& token) override;
    Return<void> onError(uint64_t deviceId, FingerprintError error, int32_t vendorCode) override;
    Return<void> onRemoved(uint64_t deviceId, uint32_t fingerId, uint32_t groupId,
                           uint32_t remaining) override;
    Return<void> onEnumerate(uint64_t deviceId, uint32_t fingerId, uint32_t groupId,
                             uint32_t remaining) override;

  private:
    sp<IBiometricsFingerprint> mService;
    bool checkSessionState() const;

    Error VendorErrorFilter(FingerprintError error, int32_t* vendorCode);
    AcquiredInfo VendorAcquiredFilter(FingerprintAcquiredInfo info);
    int32_t RequestStatusToInt(RequestStatus error);
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
