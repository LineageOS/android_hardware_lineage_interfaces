/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FingerprintEngine.h"

namespace aidl::android::hardware::biometrics::fingerprint {

FingerprintEngine::FingerprintEngine(std::shared_ptr<FingerprintConfig> config)
    : mConfig(std::move(config)) {
    ALOGV("FingerprintEngine()");
}

FingerprintEngine::~FingerprintEngine() {
    ALOGV("~FingerprintEngine()");
}

void FingerprintEngine::startSession(std::shared_ptr<ISessionCallback> cb, int userId) {
    mCb = std::move(cb);
    mUserId = userId;
}

void FingerprintEngine::endSession() {
    mCb->onSessionClosed();
}

ndk::ScopedAStatus FingerprintEngine::detectInteractionImpl() {
    ALOGD("Detect interaction is not supported");
    if (mCb) {
        mCb->onError(Error::UNABLE_TO_PROCESS, 0 /* vendorCode */);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FingerprintEngine::resetLockoutImpl(const HardwareAuthToken& /* hat */) {
    clearLockout(true);
    if (mIsLockoutTimerStarted) mIsLockoutTimerAborted = true;

    return ndk::ScopedAStatus::ok();
}

bool FingerprintEngine::checkSensorLockout() {
    LockoutTracker::LockoutMode lockoutMode = mLockoutTracker.getMode();
    if (lockoutMode == LockoutTracker::LockoutMode::kPermanent) {
        ALOGE("Fail: lockout permanent");
        mCb->onLockoutPermanent();
        mIsLockoutTimerAborted = true;
        return true;
    }
    if (lockoutMode == LockoutTracker::LockoutMode::kTimed) {
        int64_t timeLeft = mLockoutTracker.getLockoutTimeLeft();
        ALOGE("Fail: lockout timed: %ld", timeLeft);
        mCb->onLockoutTimed(timeLeft);
        if (!mIsLockoutTimerStarted) startLockoutTimer(timeLeft);
        return true;
    }
    return false;
}

void FingerprintEngine::clearLockout(bool clearAttemptCounter) {
    mLockoutTracker.reset(clearAttemptCounter);
    mCb->onLockoutCleared();
}

void FingerprintEngine::startLockoutTimer(int64_t timeout) {
    std::function<void()> action = std::bind(&FingerprintEngine::lockoutTimerExpired, this);
    std::thread([timeout, action]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        action();
    }).detach();

    mIsLockoutTimerStarted = true;
}

void FingerprintEngine::lockoutTimerExpired() {
    if (!mIsLockoutTimerAborted) clearLockout(false);

    mIsLockoutTimerStarted = false;
    mIsLockoutTimerAborted = false;
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
