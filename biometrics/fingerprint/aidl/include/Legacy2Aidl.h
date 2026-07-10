/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/keymaster/HardwareAuthToken.h>

#include <hardware/hw_auth_token.h>

#include <hidl/HidlSupport.h>

#include <endian.h>

namespace aidl::android::hardware::biometrics::fingerprint {

inline void translate(const ::aidl::android::hardware::keymaster::HardwareAuthToken& authToken,
                      hw_auth_token_t& hat) {
    hat.challenge = authToken.challenge;
    hat.user_id = authToken.userId;
    hat.authenticator_id = authToken.authenticatorId;
    // these are in host order: translate to network order
    hat.authenticator_type = htobe32(static_cast<uint32_t>(authToken.authenticatorType));
    hat.timestamp = htobe64(authToken.timestamp.milliSeconds);
    std::copy(authToken.mac.begin(), authToken.mac.end(), hat.hmac);
}

inline void translate(const hw_auth_token_t& hat,
                      ::aidl::android::hardware::keymaster::HardwareAuthToken& authToken) {
    authToken.challenge = hat.challenge;
    authToken.userId = hat.user_id;
    authToken.authenticatorId = hat.authenticator_id;
    // these are in network order: translate to host
    authToken.authenticatorType =
            static_cast<::aidl::android::hardware::keymaster::HardwareAuthenticatorType>(
                    be32toh(hat.authenticator_type));
    authToken.timestamp.milliSeconds = be64toh(hat.timestamp);
    authToken.mac.insert(authToken.mac.begin(), std::begin(hat.hmac), std::end(hat.hmac));
}

inline void translate(const ::aidl::android::hardware::keymaster::HardwareAuthToken& authToken,
                      ::android::hardware::hidl_array<uint8_t, 69>& hat_hidl) {
    hw_auth_token_t hat;
    translate(authToken, hat);
    hat_hidl = ::android::hardware::hidl_array<uint8_t, 69>(reinterpret_cast<uint8_t*>(&hat));
}

inline void translate(const ::android::hardware::hidl_vec<uint8_t>& token,
                      ::aidl::android::hardware::keymaster::HardwareAuthToken& authToken) {
    if (token.size() >= sizeof(hw_auth_token_t)) {
        hw_auth_token_t hat;
        std::memcpy(&hat, token.data(), sizeof(hw_auth_token_t));
        translate(hat, authToken);
    }
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
