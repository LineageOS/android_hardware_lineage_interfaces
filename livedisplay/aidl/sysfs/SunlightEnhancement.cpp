/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "SunlightEnhancementService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <livedisplay/sysfs/SunlightEnhancement.h>

using ::android::base::ReadFileToString;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

static constexpr const char* kSunlightEnhancementHBMPaths[] = {
        "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_hbm",
        "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/hbm",
        "/sys/class/graphics/fb0/hbm",
};

static constexpr const char* kSunlightEnhancementSREPaths[] = {
        "/sys/class/graphics/fb0/sre",
};

SunlightEnhancement::SunlightEnhancement() {
    file_ = nullptr;
    for (const auto& path : kSunlightEnhancementHBMPaths) {
        if (!access(path, R_OK | W_OK)) {
            file_ = path;
            enabled_mode_ = 1;
            break;
        }
    }

    for (const auto& path : kSunlightEnhancementSREPaths) {
        if (!access(path, R_OK | W_OK)) {
            file_ = path;
            enabled_mode_ = 2;
            break;
        }
    }
}

ndk::ScopedAStatus SunlightEnhancement::getEnabled(bool* _aidl_return) {
    if (file_ == nullptr) {
        *_aidl_return = false;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::string tmp;
    int32_t contents = 0;

    if (ReadFileToString(file_, &tmp)) {
        contents = std::stoi(Trim(tmp));
    }

    *_aidl_return = contents > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SunlightEnhancement::setEnabled(bool enabled) {
    if (file_ == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (!WriteStringToFile(enabled ? std::to_string(enabled_mode_) : "0", file_, true)) {
        LOG(ERROR) << "Failed to set SunlightEnhancement state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
