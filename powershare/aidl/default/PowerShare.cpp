/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.powershare-service.default"

#include "PowerShare.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

using ::android::base::ReadFileToString;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace lineage {
namespace powershare {

ndk::ScopedAStatus PowerShare::isEnabled(bool* _aidl_return) {
    std::string value;
    if (!ReadFileToString(POWERSHARE_PATH, &value)) {
        LOG(ERROR) << "Failed to read current PowerShare state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = Trim(value) == POWERSHARE_ENABLED;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::setEnabled(bool enable) {
    std::string value = enable ? POWERSHARE_ENABLED : POWERSHARE_DISABLED;
    if (!WriteStringToFile(value, POWERSHARE_PATH, true)) {
        LOG(ERROR) << "Failed to write PowerShare state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
    }

    return ndk::ScopedAStatus::ok();
}

// These stub implementations are currently unused.
ndk::ScopedAStatus PowerShare::getMinBattery(int32_t* _aidl_return) {
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::setMinBattery(int32_t minBattery) {
    return ndk::ScopedAStatus::ok();
}

}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
