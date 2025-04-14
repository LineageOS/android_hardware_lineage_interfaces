/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FingerprintEngine.h"

#include <android-base/logging.h>

#include <fingerprint.sysprop.h>

namespace aidl::android::hardware::biometrics::fingerprint {

FingerprintEngine::FingerprintEngine() {}

void FingerprintEngine::generateChallengeImpl(ISessionCallback* cb) {}

void FingerprintEngine::revokeChallengeImpl(ISessionCallback* cb, int64_t challenge) {}

void FingerprintEngine::enrollImpl(ISessionCallback* cb, const keymaster::HardwareAuthToken& hat,
                                   const std::future<void>& cancel) {}

void FingerprintEngine::authenticateImpl(ISessionCallback* cb, int64_t operationId,
                                         const std::future<void>& cancel) {}

void FingerprintEngine::detectInteractionImpl(ISessionCallback* cb,
                                              const std::future<void>& cancel) {}

void FingerprintEngine::enumerateEnrollmentsImpl(ISessionCallback* cb) {}

void FingerprintEngine::removeEnrollmentsImpl(ISessionCallback* cb,
                                              const std::vector<int32_t>& enrollmentIds) {}

void FingerprintEngine::getAuthenticatorIdImpl(ISessionCallback* cb) {}

void FingerprintEngine::invalidateAuthenticatorIdImpl(ISessionCallback* cb) {}

void FingerprintEngine::resetLockoutImpl(ISessionCallback* cb,
                                         const keymaster::HardwareAuthToken& hat) {}

ndk::ScopedAStatus FingerprintEngine::onPointerDownImpl(int32_t /*pointerId*/, int32_t /*x*/,
                                                        int32_t /*y*/, float /*minor*/,
                                                        float /*major*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngine::onPointerUpImpl(int32_t /*pointerId*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngine::onUiReadyImpl() {
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
