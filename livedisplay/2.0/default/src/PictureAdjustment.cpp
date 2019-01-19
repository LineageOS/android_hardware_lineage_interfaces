/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PictureAdjustment.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

PictureAdjustment::PictureAdjustment(sp<Color> colorImpl) {
    mColorImpl = colorImpl;
}

bool PictureAdjustment::isSupported() {
    if (mColorImpl != nullptr) {
        return mColorImpl->check(Feature::PICTURE_ADJUSTMENT);
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
       mColorImpl->getHueRange(_hidl_cb);
    } else {
        _hidl_cb(Range());
    }

    return Void();
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
       mColorImpl->getSaturationRange(_hidl_cb);
    } else {
        _hidl_cb(FloatRange());
    }

    return Void();
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
       mColorImpl->getIntensityRange(_hidl_cb);
    } else {
        _hidl_cb(FloatRange());
    }

    return Void();
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
       mColorImpl->getContrastRange(_hidl_cb);
    } else {
        _hidl_cb(FloatRange());
    }

    return Void();
}

Return<void> PictureAdjustment::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getSaturationThresholdRange(_hidl_cb);
    } else {
        _hidl_cb(FloatRange());
    }

    return Void();
}

Return<void> PictureAdjustment::getAdjustment(getAdjustment_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getPictureAdjustment(_hidl_cb);
    } else {
        _hidl_cb(HSIC());
    }

    return Void();
}

Return<void> PictureAdjustment::getDefaultAdjustment(getDefaultAdjustment_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getDefaultPictureAdjustment(_hidl_cb);
    } else {
        _hidl_cb(HSIC());
    }

    return Void();
}

Return<bool> PictureAdjustment::setAdjustment(const HSIC& hsic) {
    if (mColorImpl != nullptr) {
        return mColorImpl->setPictureAdjustment(hsic);
    }

    return false;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
