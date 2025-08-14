/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnPictureAdjustment.h>
#include <android-base/macros.h>

#include "SDMController.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {

class PictureAdjustment : public BnPictureAdjustment {
  public:
    explicit PictureAdjustment(std::shared_ptr<SDMController> controller);

    void updateDefaultPictureAdjustment();

    // Methods from ::aidl::vendor::lineage::livedisplay::BnPictureAdjustment follow.
    ndk::ScopedAStatus getHueRange(FloatRange* _aidl_return) override;
    ndk::ScopedAStatus getSaturationRange(FloatRange* _aidl_return) override;
    ndk::ScopedAStatus getIntensityRange(FloatRange* _aidl_return) override;
    ndk::ScopedAStatus getContrastRange(FloatRange* _aidl_return) override;
    ndk::ScopedAStatus getSaturationThresholdRange(FloatRange* _aidl_return) override;
    ndk::ScopedAStatus getPictureAdjustment(HSIC* _aidl_return) override;
    ndk::ScopedAStatus getDefaultPictureAdjustment(HSIC* _aidl_return) override;
    ndk::ScopedAStatus setPictureAdjustment(const HSIC& hsic) override;

  private:
    std::shared_ptr<SDMController> controller_;
    HSIC default_pa_ = {};

    bool isReady();
    std::optional<HSIC> getPictureAdjustmentInternal();

    DISALLOW_IMPLICIT_CONSTRUCTORS(PictureAdjustment);
};

}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
