/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnSunlightEnhancement.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sysfs {

class SunlightEnhancement : public BnSunlightEnhancement {
  public:
    SunlightEnhancement();

    // Methods from ::aidl::vendor::lineage::livedisplay::BnSunlightEnhancement follow.
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enabled) override;

  private:
    enum class SeMode : int32_t {
        NONE,
        HBM,
        SRE,
    };

    explicit SunlightEnhancement(std::pair<const char*, SeMode> init)
        : file_(init.first), enabled_mode_(init.second) {}
    bool isSupported() const;
    const std::string file_;
    const SeMode enabled_mode_;
};

}  // namespace sysfs
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
