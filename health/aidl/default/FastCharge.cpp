/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FastCharge.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <map>

#define LOG_TAG "vendor.lineage.health-service.default"

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

static std::map<FastChargeMode, std::string> fastChargeNodeValues = {
#ifdef HEALTH_FAST_CHARGE_VALUE_NONE
        {FastChargeMode::NONE, HEALTH_FAST_CHARGE_VALUE_NONE},
#endif
#ifdef HEALTH_FAST_CHARGE_VALUE_FAST_CHARGE
        {FastChargeMode::FAST_CHARGE, HEALTH_FAST_CHARGE_VALUE_FAST_CHARGE},
#endif
#ifdef HEALTH_FAST_CHARGE_VALUE_SUPER_FAST_CHARGE
        {FastChargeMode::SUPER_FAST_CHARGE, HEALTH_FAST_CHARGE_VALUE_SUPER_FAST_CHARGE},
#endif
};

ndk::ScopedAStatus FastCharge::getSupportedFastChargeModes(int64_t* _aidl_return) {
    *_aidl_return = 0;

#ifdef HEALTH_FAST_CHARGE_VALUE_NONE
    *_aidl_return |= static_cast<int>(FastChargeMode::NONE);
#endif

#ifdef HEALTH_FAST_CHARGE_VALUE_FAST_CHARGE
    *_aidl_return |= static_cast<int>(FastChargeMode::FAST_CHARGE);
#endif

#ifdef HEALTH_FAST_CHARGE_VALUE_SUPER_FAST_CHARGE
    *_aidl_return |= static_cast<int>(FastChargeMode::SUPER_FAST_CHARGE);
#endif

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FastCharge::getFastChargeMode(FastChargeMode* _aidl_return) {
#ifdef HEALTH_FAST_CHARGE_NODE
    std::string content;
    if (!android::base::ReadFileToString(HEALTH_FAST_CHARGE_NODE, &content, true)) {
        LOG(ERROR) << "Failed to read current fast charging value";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    content = android::base::Trim(content);

    auto it = std::find_if(fastChargeNodeValues.cbegin(), fastChargeNodeValues.cend(),
                           [&](const std::pair<FastChargeMode, std::string>& pair) {
                               return pair.second == content;
                           });
    if (it == fastChargeNodeValues.cend()) {
        LOG(ERROR) << "Failed to parse current fast charging value: " << content;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = it->first;
#else
    *_aidl_return = 0;
#endif

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FastCharge::setFastChargeMode(FastChargeMode in_mode,
                                                 FastChargeMode* _aidl_return) {
#ifdef HEALTH_FAST_CHARGE_NODE
    if (!android::base::WriteStringToFile(HEALTH_FAST_CHARGE_NODE, fastChargeNodeValues[in_mode])) {
        LOG(ERROR) << "Failed to write to fast charge node: " << strerror(errno);
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
#endif

    return getFastChargeMode(_aidl_return);
}

binder_status_t FastCharge::dump(int fd, const char** args, uint32_t numArgs) {
    int64_t supportedFastChargeModes;
    getSupportedFastChargeModes(&supportedFastChargeModes);

    FastChargeMode fastChargeMode;
    getFastChargeMode(&fastChargeMode);

    dprintf(fd, "Fast charge supported modes: %ld\n", supportedFastChargeModes);
    dprintf(fd, "Fast charge mode: %d\n", fastChargeMode);

    return STATUS_OK;
}

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
