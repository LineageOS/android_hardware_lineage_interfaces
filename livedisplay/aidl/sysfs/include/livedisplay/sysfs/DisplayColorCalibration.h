/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnDisplayColorCalibration.h>

#include <livedisplay/sysfs/DualStateMode.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sysfs {

class DisplayColorCalibration : public BnDisplayColorCalibration,
                                public PathManager<DisplayColorCalibration> {
  public:
    // Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayColorCalibration follow.
    ndk::ScopedAStatus getMaxValue(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getMinValue(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getCalibration(std::vector<int32_t>* _aidl_return) override;
    ndk::ScopedAStatus setCalibration(const std::vector<int32_t>& rgb) override;
};

}  // namespace sysfs
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
