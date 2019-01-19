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

#include "AdaptiveBacklight.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

AdaptiveBacklight::AdaptiveBacklight(sp<Color> colorImpl) {
    mColorImpl = colorImpl;
}

bool AdaptiveBacklight::isSupported() {
    if (mColorImpl != nullptr) {
        return mColorImpl->check(Feature::ADAPTIVE_BACKLIGHT);
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    if (mColorImpl != nullptr) {
        return mColorImpl->isAdaptiveBacklightEnabled();
    }

    return false;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    if (mColorImpl != nullptr) {
        return mColorImpl->setAdaptiveBacklightEnabled(enabled);
    }

    return false;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
