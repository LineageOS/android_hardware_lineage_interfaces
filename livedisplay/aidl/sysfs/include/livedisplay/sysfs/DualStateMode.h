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
struct ModeTrait {
    static constexpr const char* kName = nullptr;
    static constexpr std::array<const char*, 0> kPaths;
};

template <typename T>
class PathManager {
  protected:
    PathManager()
        : file_([]() {
              static_assert(ModeTrait<T>::kPaths.size() > 0, "ModeTrait must define kPaths");
              for (auto&& path : ModeTrait<T>::kPaths) {
                  if (!access(path, R_OK | W_OK)) {
                      return path;
                  }
              }
              LOG(FATAL) << "None of " << ModeTrait<T>::kName << " paths could be opened";
              return "";
          }()) {}
    const std::string file_;
};

template <typename T>
class DualStateMode : public T, public PathManager<DualStateMode<T>> {
  public:
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override {
        std::string tmp;
        if (!ReadFileToString(this->file_, &tmp, true)) {
            *_aidl_return = false;
            return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
        }

        *_aidl_return = std::stoi(Trim(tmp)) > 0;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus setEnabled(bool enabled) override {
        if (!WriteStringToFile(std::to_string(enabled), this->file_, true)) {
            static_assert(ModeTrait<DualStateMode<T>>::kName != nullptr,
                          "ModeTrait must define kName");
            LOG(ERROR) << "Failed to set " << ModeTrait<DualStateMode<T>>::kName << " state";
            return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
        }

        return ndk::ScopedAStatus::ok();
    }
};

#define DEFINE_DUAL_STATE_MODE(Class, ...)                  \
    using Class = DualStateMode<Bn##Class>;                 \
    template <>                                             \
    struct ModeTrait<Class> {                               \
        static constexpr const char* kName = #Class;        \
        static constexpr std::array kPaths = {__VA_ARGS__}; \
    };

DEFINE_DUAL_STATE_MODE(AdaptiveBacklight, "/sys/class/graphics/fb0/acl",
                       "/sys/class/graphics/fb0/cabc",
                       "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/acl",
                       "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/cabc")

DEFINE_DUAL_STATE_MODE(AutoContrast, "/sys/class/graphics/fb0/aco")

DEFINE_DUAL_STATE_MODE(AntiFlicker, "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dc")

DEFINE_DUAL_STATE_MODE(ColorEnhancement, "/sys/class/graphics/fb0/color_enhance")

DEFINE_DUAL_STATE_MODE(ReadingEnhancement, "/sys/class/graphics/fb0/reading_mode")

}  // namespace sysfs
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
