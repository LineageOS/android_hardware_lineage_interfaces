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

PictureAdjustment::PictureAdjustment() {
    mColorHal = IColor::getService();
    if (mColorHal == nullptr) {
        return nullptr;
    }
}

// Methods from IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    return mColorHal->getHueRange(_hidl_cb);
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    return mColorHal->getSaturationRange(_hidl_cb);
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    return mColorHal->getIntensityRange(_hidl_cb);
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    return mColorHal->getContrastRange(_hidl_cb);
}

Return<void> PictureAdjustment::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    return mColorHal->getSaturationThresholdRange(_hidl_cb);
}

Return<void> PictureAdjustment::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    return mColorHal->getPictureAdjustment(_hidl_cb);
}

Return<void> PictureAdjustment::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
    return mColorHal->getDefaultPictureAdjustment(_hidl_cb);
}

Return<bool> PictureAdjustment::setPictureAdjustment(const HSIC& hsic) {
    return mColorHal->setPictureAdjustment(hsic);
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
