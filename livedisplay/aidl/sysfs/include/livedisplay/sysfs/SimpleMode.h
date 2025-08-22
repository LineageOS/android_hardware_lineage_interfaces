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
namespace sysfs {

template <typename T>
struct SimpleModeTrait {
    static constexpr const char* kPaths[] = {};
    static constexpr const char* kInterface = nullptr;
};

template <typename T>
class PathManager {
  protected:
    PathManager()
        : file_([]() {
              for (auto&& path : SimpleModeTrait<T>::kPaths) {
                  if (!access(path, R_OK | W_OK)) {
                      return path;
                  }
              }
              return "";
          }()) {}
    bool isSupported() const { return !file_.empty(); }
    const std::string file_;
};

template <typename T>
class SimpleMode : public T, public PathManager<SimpleMode<T>> {
  public:
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override {
        if (!this->isSupported()) {
            *_aidl_return = false;
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }

        std::string tmp;
        if (!ReadFileToString(this->file_, &tmp, true)) {
            *_aidl_return = false;
            return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
        }

        *_aidl_return = std::stoi(Trim(tmp)) > 0;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus setEnabled(bool enabled) override {
        if (!this->isSupported()) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }

        if (!WriteStringToFile(std::to_string(enabled), this->file_, true)) {
            LOG(ERROR) << "Failed to set " << SimpleModeTrait<T>::kInterface << " state";
            return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
        }

        return ndk::ScopedAStatus::ok();
    }
};

#define DEFINE_SIMPLE_MODE_PATHS(Class, ...)                   \
    template <>                                                \
    struct SimpleModeTrait<SimpleMode<Bn##Class>> {            \
        static constexpr const char* kPaths[] = {__VA_ARGS__}; \
        static constexpr const char* kInterface = #Class;      \
    };                                                         \
    using Class = SimpleMode<Bn##Class>;

DEFINE_SIMPLE_MODE_PATHS(AdaptiveBacklight, "/sys/class/graphics/fb0/acl",
                         "/sys/class/graphics/fb0/cabc",
                         "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/cabc",
                         "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_acl",
                         "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_cabc")

DEFINE_SIMPLE_MODE_PATHS(AutoContrast, "/sys/class/graphics/fb0/aco")

DEFINE_SIMPLE_MODE_PATHS(AntiFlicker, "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dc",
                         "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_dc")

DEFINE_SIMPLE_MODE_PATHS(ColorEnhancement, "/sys/class/graphics/fb0/color_enhance")

DEFINE_SIMPLE_MODE_PATHS(ReadingEnhancement, "/sys/class/graphics/fb0/reading_mode")

}  // namespace sysfs
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
