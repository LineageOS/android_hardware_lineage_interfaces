/*
 * SPDX-FileCopyrightText: 2014-2019 The Android Open Source Project
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dlfcn.h>
#include <endian.h>
#include <limits>

#include <android-base/logging.h>

#include <gatekeeper/password_handle.h>

#include "Gatekeeper.h"

namespace aidl {
namespace android {
namespace hardware {
namespace gatekeeper {

Gatekeeper::Gatekeeper() {
    int ret = hw_get_module_by_class(GATEKEEPER_HARDWARE_MODULE_ID, NULL, &mModule);
    mDevice = nullptr;

    if (!ret) {
        ret = gatekeeper_open(mModule, &mDevice);
    }

    if (ret < 0) {
        LOG(ERROR) << "Unable to open GateKeeper HAL.";
        abort();
    }
}

Gatekeeper::~Gatekeeper() {
    if (mDevice != nullptr) {
        int ret = gatekeeper_close(mDevice);
        if (ret < 0) {
            LOG(ERROR) << "Unable to close GateKeeper HAL.";
        }
    }
    dlclose(mModule->dso);
}

void legacyAuthToken2AidlHWToken(
        const hw_auth_token_t* authToken,
        android::hardware::security::keymint::HardwareAuthToken* aidlToken) {
    aidlToken->challenge = authToken->challenge;
    aidlToken->userId = authToken->user_id;
    aidlToken->authenticatorId = authToken->authenticator_id;
    // these are in network order: translate to host
    aidlToken->authenticatorType =
            static_cast<android::hardware::security::keymint::HardwareAuthenticatorType>(
                    be32toh(authToken->authenticator_type));
    aidlToken->timestamp.milliSeconds = be64toh(authToken->timestamp);
    aidlToken->mac.insert(aidlToken->mac.begin(), std::begin(authToken->hmac),
                          std::end(authToken->hmac));
}

::ndk::ScopedAStatus Gatekeeper::enroll(int32_t uid,
                                        const std::vector<uint8_t>& currentPasswordHandle,
                                        const std::vector<uint8_t>& currentPassword,
                                        const std::vector<uint8_t>& desiredPassword,
                                        GatekeeperEnrollResponse* rsp) {
    uint8_t* enrolled_password_handle = nullptr;
    uint32_t enrolled_password_handle_length = 0;

    if (desiredPassword.size() == 0) {
        return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_GENERAL_FAILURE));
    }

    int ret = mDevice->enroll(
            mDevice, uid, currentPasswordHandle.data(), currentPasswordHandle.size(),
            currentPassword.data(), currentPassword.size(), desiredPassword.data(),
            desiredPassword.size(), &enrolled_password_handle, &enrolled_password_handle_length);

    if (!ret) {
        password_handle_t* _enrolled_password_handle =
                reinterpret_cast<password_handle_t*>(enrolled_password_handle);
        *rsp = {STATUS_OK,
                0,
                static_cast<int64_t>(_enrolled_password_handle->user_id),
                {enrolled_password_handle,
                 (enrolled_password_handle + enrolled_password_handle_length)}};
    } else if (ret > 0) {
        *rsp = {ERROR_RETRY_TIMEOUT, ret, 0, {}};
    } else {
        return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_GENERAL_FAILURE));
    }
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Gatekeeper::verify(int32_t uid, int64_t challenge,
                                        const std::vector<uint8_t>& enrolledPasswordHandle,
                                        const std::vector<uint8_t>& providedPassword,
                                        GatekeeperVerifyResponse* rsp) {
    uint8_t* auth_token = nullptr;
    uint32_t auth_token_length = 0;
    bool request_reenroll = false;

    int ret = mDevice->verify(mDevice, uid, challenge, enrolledPasswordHandle.data(),
                              enrolledPasswordHandle.size(), providedPassword.data(),
                              providedPassword.size(), &auth_token, &auth_token_length,
                              &request_reenroll);
    if (!ret) {
        hw_auth_token_t* _auth_token = reinterpret_cast<hw_auth_token_t*>(auth_token);
        // On Success, return GatekeeperVerifyResponse with Success Status,
        // timeout{0} and valid HardwareAuthToken.
        *rsp = {request_reenroll ? STATUS_REENROLL : STATUS_OK, 0, {}};
        // Convert the hw_auth_token_t to HardwareAuthToken in the response.
        legacyAuthToken2AidlHWToken(_auth_token, &rsp->hardwareAuthToken);
    } else if (ret > 0) {
        *rsp = {ERROR_RETRY_TIMEOUT, ret, {}};
    } else {
        return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_GENERAL_FAILURE));
    }

    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Gatekeeper::deleteUser(int32_t uid) {
    if (mDevice->delete_user) {
        int ret = mDevice->delete_user(mDevice, uid);
        if (!ret) {
            return ndk::ScopedAStatus::ok();
        } else {
            return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_GENERAL_FAILURE));
        }
    } else {
        return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_NOT_IMPLEMENTED));
    }
}

::ndk::ScopedAStatus Gatekeeper::deleteAllUsers() {
    if (mDevice->delete_all_users) {
        int ret = mDevice->delete_all_users(mDevice);
        if (!ret) {
            return ndk::ScopedAStatus::ok();
        } else {
            return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_GENERAL_FAILURE));
        }
    } else {
        return ndk::ScopedAStatus(AStatus_fromServiceSpecificError(ERROR_NOT_IMPLEMENTED));
    }
}

}  // namespace gatekeeper
}  // namespace hardware
}  // namespace android
}  // namespace aidl
