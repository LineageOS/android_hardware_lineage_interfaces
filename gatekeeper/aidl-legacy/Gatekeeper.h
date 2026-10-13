/*
 * SPDX-FileCopyrightText: 2014-2019 The Android Open Source Project
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <aidl/android/hardware/gatekeeper/BnGatekeeper.h>
#include <gatekeeper/password_handle.h>
#include <hardware/gatekeeper.h>
#include <hardware/hardware.h>
#include <hardware/hw_auth_token.h>

namespace aidl {
namespace android {
namespace hardware {
namespace gatekeeper {

using aidl::android::hardware::gatekeeper::GatekeeperEnrollResponse;
using aidl::android::hardware::gatekeeper::GatekeeperVerifyResponse;
using ::gatekeeper::password_handle_t;

class Gatekeeper : public BnGatekeeper {
  public:
    explicit Gatekeeper();
    ~Gatekeeper();
    /**
     * Enrolls password_payload, which should be derived from a user selected pin
     * or password, with the authentication factor private key used only for
     * enrolling authentication factor data.
     *
     * Returns: 0 on success or an error code less than 0 on error.
     * On error, enrolled_password_handle will not be allocated.
     */
    ::ndk::ScopedAStatus enroll(int32_t uid, const std::vector<uint8_t>& currentPasswordHandle,
                                const std::vector<uint8_t>& currentPassword,
                                const std::vector<uint8_t>& desiredPassword,
                                GatekeeperEnrollResponse* _aidl_return) override;
    /**
     * Verifies provided_password matches enrolled_password_handle.
     *
     * Implementations of this module may retain the result of this call
     * to attest to the recency of authentication.
     *
     * On success, writes the address of a verification token to auth_token,
     * usable to attest password verification to other trusted services. Clients
     * may pass NULL for this value.
     *
     * Returns: 0 on success or an error code less than 0 on error
     * On error, verification token will not be allocated
     */
    ::ndk::ScopedAStatus verify(int32_t uid, int64_t challenge,
                                const std::vector<uint8_t>& enrolledPasswordHandle,
                                const std::vector<uint8_t>& providedPassword,
                                GatekeeperVerifyResponse* _aidl_return) override;
    ::ndk::ScopedAStatus deleteAllUsers() override;
    ::ndk::ScopedAStatus deleteUser(int32_t uid) override;

  private:
    gatekeeper_device_t* mDevice;
    const hw_module_t* mModule;
};

}  // namespace gatekeeper
}  // namespace hardware
}  // namespace android
}  // namespace aidl
