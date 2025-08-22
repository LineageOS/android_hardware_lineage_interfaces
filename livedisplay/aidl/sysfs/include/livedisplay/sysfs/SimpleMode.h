/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnAdaptiveBacklight.h>
#include <aidl/vendor/lineage/livedisplay/BnAntiFlicker.h>
#include <aidl/vendor/lineage/livedisplay/BnAutoContrast.h>
#include <aidl/vendor/lineage/livedisplay/BnColorEnhancement.h>
#include <aidl/vendor/lineage/livedisplay/BnReadingEnhancement.h>
#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

using ::android::base::ReadFileToString;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

template <typename T>
struct SimpleModeTrait {
    static constexpr const char* kPaths[] = {nullptr};
    static constexpr const char* kInterface = "Unknown";
};

template <typename T>
class SimpleMode : public T {
  public:
    SimpleMode() {
        file_ = nullptr;
        for (const auto& path : SimpleModeTrait<T>::kPaths) {
            if (path && !access(path, R_OK | W_OK)) {
                file_ = path;
                break;
            }
        }
    }

    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override {
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

    ndk::ScopedAStatus setEnabled(bool enabled) override {
        if (file_ == nullptr) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }

        if (!WriteStringToFile(std::to_string(enabled), file_, true)) {
            LOG(ERROR) << "Failed to set " << SimpleModeTrait<T>::kInterface << " state";
            return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
        }

        return ndk::ScopedAStatus::ok();
    }

  private:
    const char* file_;
};

template <>
struct SimpleModeTrait<BnAdaptiveBacklight> {
    static constexpr const char* kPaths[] = {
            "/sys/class/graphics/fb0/acl",
            "/sys/class/graphics/fb0/cabc",
            "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/cabc",
            "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_acl",
            "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_cabc",
    };
    static constexpr const char* kInterface = "AdaptiveBacklight";
};
using AdaptiveBacklight = SimpleMode<BnAdaptiveBacklight>;

template <>
struct SimpleModeTrait<BnAutoContrast> {
    static constexpr const char* kPaths[] = {
            "/sys/class/graphics/fb0/aco",
    };
    static constexpr const char* kInterface = "AutoContrast";
};
using AutoContrast = SimpleMode<BnAutoContrast>;

template <>
struct SimpleModeTrait<BnAntiFlicker> {
    static constexpr const char* kPaths[] = {
            "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dc",
            "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_dc",
    };
    static constexpr const char* kInterface = "AntiFlicker";
};
using AntiFlicker = SimpleMode<BnAntiFlicker>;

template <>
struct SimpleModeTrait<BnColorEnhancement> {
    static constexpr const char* kPaths[] = {
            "/sys/class/graphics/fb0/color_enhance",
    };
    static constexpr const char* kInterface = "ColorEnhancement";
};
using ColorEnhancement = SimpleMode<BnColorEnhancement>;

template <>
struct SimpleModeTrait<BnReadingEnhancement> {
    static constexpr const char* kPaths[] = {
            "/sys/class/graphics/fb0/reading_mode",
    };
    static constexpr const char* kInterface = "ReadingEnhancement";
};
using ReadingEnhancement = SimpleMode<BnReadingEnhancement>;

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
