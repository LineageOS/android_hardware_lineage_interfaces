/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-impl-sdm"

#include "livedisplay/sdm/PictureAdjustment.h"
#include "livedisplay/sdm/Utils.h"

#include <android-base/logging.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {

using ::android::OK;

PictureAdjustment::PictureAdjustment(std::shared_ptr<SDMController> controller)
    : controller_(controller) {
    if (!isReady()) {
        LOG(FATAL) << "PictureAdjustment backend not ready, exiting.";
    }
}

bool PictureAdjustment::isReady() {
    if (utils::CheckFeatureVersion(controller_, utils::FEATURE_VER_SW_PA_API) != OK) {
        return false;
    }

    HsicRanges r{};
    if (controller_->getGlobalPaRange(&r) != OK) {
        return false;
    }

    return r.hue.max != 0 && r.hue.min != 0 && r.saturation.max != 0.f && r.saturation.min != 0.f &&
           r.intensity.max != 0.f && r.intensity.min != 0.f && r.contrast.max != 0.f &&
           r.contrast.min != 0.f;
}

HSIC PictureAdjustment::getPictureAdjustmentInternal() {
    HsicConfig config{};

    if (controller_->getGlobalPaConfig(&config) == OK) {
        return HSIC{static_cast<float>(config.data.hue), config.data.saturation,
                    config.data.intensity, config.data.contrast, config.data.saturation_threshold};
    }

    return HSIC{};
}

void PictureAdjustment::updateDefaultPictureAdjustment() {
    default_pa_ = getPictureAdjustmentInternal();
}

// Methods from ::aidl::vendor::lineage::livedisplay::BnPictureAdjustment follow.
ndk::ScopedAStatus PictureAdjustment::getHueRange(FloatRange* _aidl_return) {
    FloatRange range{};
    HsicRanges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.hue.max;
        range.min = r.hue.min;
        range.step = r.hue.step;
    }

    *_aidl_return = range;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::getSaturationRange(FloatRange* _aidl_return) {
    FloatRange range{};
    HsicRanges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.saturation.max;
        range.min = r.saturation.min;
        range.step = r.saturation.step;
    }

    *_aidl_return = range;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::getIntensityRange(FloatRange* _aidl_return) {
    FloatRange range{};
    HsicRanges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.intensity.max;
        range.min = r.intensity.min;
        range.step = r.intensity.step;
    }

    *_aidl_return = range;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::getContrastRange(FloatRange* _aidl_return) {
    FloatRange range{};
    HsicRanges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.contrast.max;
        range.min = r.contrast.min;
        range.step = r.contrast.step;
    }

    *_aidl_return = range;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::getSaturationThresholdRange(FloatRange* _aidl_return) {
    FloatRange range{};
    HsicRanges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.saturation_threshold.max;
        range.min = r.saturation_threshold.min;
        range.step = r.saturation_threshold.step;
    }

    *_aidl_return = range;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::getPictureAdjustment(HSIC* _aidl_return) {
    *_aidl_return = getPictureAdjustmentInternal();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::getDefaultPictureAdjustment(HSIC* _aidl_return) {
    *_aidl_return = default_pa_;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PictureAdjustment::setPictureAdjustment(const HSIC& hsic) {
    HsicConfig config = {0,
                         {static_cast<int32_t>(hsic.hue), hsic.saturation, hsic.intensity,
                          hsic.contrast, hsic.saturationThreshold}};

    if (controller_->setGlobalPaConfig(&config) != OK) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
