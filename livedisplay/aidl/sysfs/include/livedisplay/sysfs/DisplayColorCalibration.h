/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnDisplayColorCalibration.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

class DisplayColorCalibration : public BnDisplayColorCalibration {
  public:
    DisplayColorCalibration();

    // Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayColorCalibration follow.
    ndk::ScopedAStatus getMaxValue(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getMinValue(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getCalibration(std::vector<int32_t>* _aidl_return) override;
    ndk::ScopedAStatus setCalibration(const std::vector<int32_t>& rgb) override;

  private:
    const char* file_;
};

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
