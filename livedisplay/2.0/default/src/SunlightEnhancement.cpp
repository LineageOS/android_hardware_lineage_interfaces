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

#include "SunlightEnhancement.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

SunlightEnhancement::SunlightEnhancement(sp<Color> colorImpl) {
    mColorImpl = colorImpl;
}

bool SunlightEnhancement::isSupported() {
    if (mColorImpl != nullptr) {
        return mColorImpl->check(Feature::OUTDOOR_MODE);
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::ISunlightEnhancement follow.
Return<bool> SunlightEnhancement::isEnabled() {
    if (mColorImpl != nullptr) {
        return mColorImpl->isOutdoorModeEnabled();
    }

    return false;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    if (mColorImpl != nullptr) {
        return mColorImpl->setOutdoorModeEnabled(enabled);
    }

    return false;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
