/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include "Legacy2Aidl.h"
#include "Session.h"

#include "CancellationSignal.h"

namespace aidl::android::hardware::biometrics::fingerprint {

void onClientDeath(void* cookie) {
    ALOGI("FingerprintService has died");
    Session* session = static_cast<Session*>(cookie);
    if (session && !session->isClosed()) {
        session->close();
    }
}

Session::Session(std::shared_ptr<FingerprintEngine> engine,
                 int userId, std::shared_ptr<ISessionCallback> cb)
    : mEngine(engine), mUserId(userId) {
    mDeathRecipient = AIBinder_DeathRecipient_new(onClientDeath);

    mEngine->startSession(cb, userId);
}

ndk::ScopedAStatus Session::generateChallenge() {
    return mEngine->generateChallengeImpl();
}

ndk::ScopedAStatus Session::revokeChallenge(int64_t challenge) {
    return mEngine->revokeChallengeImpl(challenge);
}

ndk::ScopedAStatus Session::enroll(const HardwareAuthToken& hat,
                                   std::shared_ptr<ICancellationSignal>* out) {
    ndk::ScopedAStatus status = mEngine->enrollImpl(hat);
    *out = SharedRefBase::make<CancellationSignal>(this);
    return status;
}

ndk::ScopedAStatus Session::authenticate(int64_t operationId,
                                         std::shared_ptr<ICancellationSignal>* out) {
    ndk::ScopedAStatus status = mEngine->authenticateImpl(operationId);
    *out = SharedRefBase::make<CancellationSignal>(this);
    return status;
}

ndk::ScopedAStatus Session::detectInteraction(std::shared_ptr<ICancellationSignal>* out) {
    mEngine->detectInteractionImpl();
    *out = SharedRefBase::make<CancellationSignal>(this);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::enumerateEnrollments() {
    return mEngine->enumerateEnrollmentsImpl();
}

ndk::ScopedAStatus Session::removeEnrollments(const std::vector<int32_t>& enrollmentIds) {
    return mEngine->removeEnrollmentsImpl(enrollmentIds);
}

ndk::ScopedAStatus Session::getAuthenticatorId() {
    return mEngine->getAuthenticatorIdImpl();
}

ndk::ScopedAStatus Session::invalidateAuthenticatorId() {
    return mEngine->invalidateAuthenticatorIdImpl();
}

ndk::ScopedAStatus Session::resetLockout(const HardwareAuthToken& hat) {
    return mEngine->resetLockoutImpl(hat);
}

ndk::ScopedAStatus Session::onPointerDown(int32_t pointerId, int32_t x, int32_t y,
                                          float minor, float major) {
    return mEngine->onPointerDownImpl(pointerId, x, y, minor, major);
}

ndk::ScopedAStatus Session::onPointerUp(int32_t pointerId) {
    return mEngine->onPointerUpImpl(pointerId);
}

ndk::ScopedAStatus Session::onUiReady() {
    // TODO: stub

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::authenticateWithContext(
        int64_t operationId, const common::OperationContext& /*context*/,
        std::shared_ptr<common::ICancellationSignal>* out) {
    return authenticate(operationId, out);
}

ndk::ScopedAStatus Session::enrollWithContext(const keymaster::HardwareAuthToken& hat,
                                              const common::OperationContext& /*context*/,
                                              std::shared_ptr<common::ICancellationSignal>* out) {
    return enroll(hat, out);
}

ndk::ScopedAStatus Session::detectInteractionWithContext(
        const common::OperationContext& /*context*/,
        std::shared_ptr<common::ICancellationSignal>* out) {
    return detectInteraction(out);
}

ndk::ScopedAStatus Session::onPointerDownWithContext(const PointerContext& context) {
    return onPointerDown(context.pointerId, context.x, context.y, context.minor, context.major);
}

ndk::ScopedAStatus Session::onPointerUpWithContext(const PointerContext& context) {
    return onPointerUp(context.pointerId);
}

ndk::ScopedAStatus Session::onContextChanged(const common::OperationContext& /*context*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::onPointerCancelWithContext(const PointerContext& /*context*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::setIgnoreDisplayTouches(bool /*shouldIgnore*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::cancel() {
    return mEngine->cancelImpl();
}

ndk::ScopedAStatus Session::close() {
    mClosed = true;
    mEngine->endSession();
    AIBinder_DeathRecipient_delete(mDeathRecipient);
    return ndk::ScopedAStatus::ok();
}

binder_status_t Session::linkToDeath(AIBinder* binder) {
    return AIBinder_linkToDeath(binder, mDeathRecipient, this);
}

bool Session::isClosed() {
    return mClosed;
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
