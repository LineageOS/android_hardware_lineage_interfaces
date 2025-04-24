/*
 * Copyright (C) 2024-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PowerShare.h"

#include <android-base/file.h>
#include <android-base/strings.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace vendor {
namespace lineage {
namespace powershare {
namespace V1_0 {
namespace implementation {

Return<bool> PowerShare::isEnabled() {
    std::string value;

    if (!ReadFileToString(POWERSHARE_PATH, &value)) {
        LOG(ERROR) << "Failed to read current powershare value";
        return false;
    }

    value = android::base::Trim(value);

    return value == POWERSHARE_ENABLED;
}

Return<bool> PowerShare::setEnabled(bool enable) {
    const auto& value = enable ? POWERSHARE_ENABLED : POWERSHARE_DISABLED;
    WriteStringToFile(value, POWERSHARE_PATH, true);
    return isEnabled();
}

Return<uint32_t> PowerShare::getMinBattery() {
    return 0;
}

Return<uint32_t> PowerShare::setMinBattery(uint32_t /*minBattery*/) {
    return getMinBattery();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
