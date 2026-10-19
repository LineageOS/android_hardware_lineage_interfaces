/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/fingerprint/ISessionCallback.h>
#include <android/log.h>
#include <hardware/hardware.h>
#include <hardware/fingerprint.h>
#include <log/log.h>
#include <future>

#include "FingerprintEngine.h"

using ::aidl::android::hardware::keymaster::HardwareAuthToken;

namespace aidl::android::hardware::biometrics::fingerprint {

class FingerprintEngineHw : public FingerprintEngine {
  public:
    FingerprintEngineHw(std::shared_ptr<FingerprintConfig> config);
    ~FingerprintEngineHw() override;

    static std::shared_ptr<FingerprintEngine> create(std::shared_ptr<FingerprintConfig> config);

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

  private:
    fingerprint_device_t* mDevice = nullptr;
    static constexpr int kVersion = FINGERPRINT_MODULE_API_VERSION_2_1;

    fingerprint_device_t* openHal();
    static void notify(const fingerprint_msg_t* msg);
    bool checkSessionState() const;

    static Error VendorErrorFilter(int32_t error, int32_t* vendorCode);
    static AcquiredInfo VendorAcquiredFilter(int32_t info, int32_t* vendorCode);
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
