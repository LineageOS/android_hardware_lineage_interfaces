/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PowerShare.h"

#include <android-base/file.h>
#include <android-base/strings.h>
#include <android-base/logging.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace aidl::vendor::lineage::powershare {

// Check if powershare is enabled. If not, return false.
ndk::ScopedAStatus PowerShare::isEnabled(bool* out_enabled) {
    std::string value;
    if (!android::base::ReadFileToString(POWERSHARE_PATH, &value)) {
        LOG(ERROR) << "Failed to read current powershare value";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *out_enabled = android::base::Trim(value) == POWERSHARE_ENABLED;
    return ndk::ScopedAStatus::ok();
}

// Enable and disable powershare.
ndk::ScopedAStatus PowerShare::setEnabled(bool in_enable, bool* _aidl_return) {
    std::string value = in_enable ? "1" : "0";
    if (!android::base::WriteStringToFile(value, POWERSHARE_PATH)) {
        LOG(ERROR) << "Failed to write powershare state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = true;
    return ndk::ScopedAStatus::ok();
}

// The stubs below are not used (yet).

// Returns the minimum battery level required to enable powershare.
ndk::ScopedAStatus PowerShare::getMinBattery(int32_t* _aidl_return) {
    *_aidl_return = 0; // placeholder
    return ndk::ScopedAStatus::ok();
}

// Set a minimum battery threshold for powershare.
ndk::ScopedAStatus PowerShare::setMinBattery(int32_t minBattery, int32_t* _aidl_return) {
    *_aidl_return = minBattery;
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::vendor::lineage::powershare
