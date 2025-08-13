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

struct {
    std::optional<std::string> node{HEALTH_FAST_CHARGE_NODE};
    std::optional<std::string> valueNone{HEALTH_FAST_CHARGE_VALUE_NONE};
    std::optional<std::string> valueFastCharge{HEALTH_FAST_CHARGE_VALUE_FAST_CHARGE};
    std::optional<std::string> valueSuperFastCharge{HEALTH_FAST_CHARGE_VALUE_SUPER_FAST_CHARGE};

    std::optional<FastChargeMode> modeToValue(const std::string& value) const {
        if (value == valueNone) return FastChargeMode::NONE;
        if (value == valueFastCharge) return FastChargeMode::FAST_CHARGE;
        if (value == valueSuperFastCharge) return FastChargeMode::SUPER_FAST_CHARGE;
        return {};
    }

    std::optional<std::string> valueToMode(FastChargeMode mode) const {
        if (mode == FastChargeMode::NONE) return valueNone;
        if (mode == FastChargeMode::FAST_CHARGE) return valueFastCharge;
        if (mode == FastChargeMode::SUPER_FAST_CHARGE) return valueSuperFastCharge;
        return {};
    }

    int64_t supportedModes() const {
        int64_t ret = 0;

        if (node) {
            if (valueNone) ret |= static_cast<int>(FastChargeMode::NONE);
            if (valueFastCharge) ret |= static_cast<int>(FastChargeMode::FAST_CHARGE);
            if (valueSuperFastCharge) ret |= static_cast<int>(FastChargeMode::SUPER_FAST_CHARGE);
        }

        return ret;
    }
} fastChargeConfig;

ndk::ScopedAStatus FastCharge::getSupportedFastChargeModes(int64_t* _aidl_return) {
    *_aidl_return = fastChargeConfig.supportedModes();

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FastCharge::getFastChargeMode(FastChargeMode* _aidl_return) {
    if (fastChargeConfig.supportedModes() > 0) {
        std::string content;
        if (!android::base::ReadFileToString(*fastChargeConfig.node, &content, true)) {
            LOG(ERROR) << "Failed to read current fast charging value";
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
        content = android::base::Trim(content);

        auto mode = fastChargeConfig.modeToValue(content);
        if (!mode) {
            LOG(ERROR) << "Failed to parse current fast charging value: " << content;
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }

        *_aidl_return = *mode;
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FastCharge::setFastChargeMode(FastChargeMode in_mode,
                                                 FastChargeMode* _aidl_return) {
    if (fastChargeConfig.supportedModes() > 0) {
        auto value = fastChargeConfig.valueToMode(in_mode);
        if (!value) {
            LOG(ERROR) << "Unsupported fast charge mode: " << static_cast<int>(in_mode);
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }

        if (!android::base::WriteStringToFile(*value, *fastChargeConfig.node)) {
            LOG(ERROR) << "Failed to write to fast charge node: " << strerror(errno);
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
    }

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
