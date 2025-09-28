/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "SunlightEnhancementService"

#include <livedisplay/sysfs/SunlightEnhancement.h>

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

SunlightEnhancement::SunlightEnhancement()
    : SunlightEnhancement([]() {
          constexpr const char* kHBMPaths[] = {
#ifdef SE_PATH
                  SE_PATH,
#else
                  "/sys/class/graphics/fb0/hbm",
                  "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dsi_display_hbm",
                  "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/hbm",
#endif
          };
          constexpr const char* kSREPaths[] = {
#ifdef SRE_PATH
                  SRE_PATH,
#else
                  "/sys/class/graphics/fb0/sre",
#endif
          };

          for (const auto& path : kHBMPaths) {
              if (!access(path, R_OK | W_OK)) {
                  return std::make_pair(path, SeMode::HBM);
              }
          }

          for (const auto& path : kSREPaths) {
              if (!access(path, R_OK | W_OK)) {
                  return std::make_pair(path, SeMode::SRE);
              }
          }

          LOG(FATAL) << "None of SunlightEnhancement paths could be opened";

          return std::make_pair("", SeMode::NONE);
      }()) {}

ndk::ScopedAStatus SunlightEnhancement::getEnabled(bool* _aidl_return) {
    std::string tmp;
    if (!ReadFileToString(this->file_, &tmp, true)) {
        *_aidl_return = false;
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
    }

    *_aidl_return = std::stoi(Trim(tmp)) > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SunlightEnhancement::setEnabled(bool enabled) {
    if (!WriteStringToFile(enabled ? std::to_string(static_cast<int32_t>(enabled_mode_)) : "0",
                           file_, true)) {
        LOG(ERROR) << "Failed to set SunlightEnhancement state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace sysfs
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
