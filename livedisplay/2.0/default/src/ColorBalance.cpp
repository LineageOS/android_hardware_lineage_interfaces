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

#include "ColorBalance.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

ColorBalance::ColorBalance(sp<Color> colorImpl) {
    mColorImpl = colorImpl;
}

bool ColorBalance::isSupported() {
    if (mColorImpl != nullptr) {
        return mColorImpl->check(Feature::COLOR_BALANCE);
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorBalance follow.
Return<void> ColorBalance::getRange(getRange_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getColorBalanceRange(_hidl_cb);
    } else {
        Range range;
        range.max = range.min = 0;
        _hidl_cb(range);
    }

    return Void();
}

Return<int32_t> ColorBalance::getValue() {
    if (mColorImpl != nullptr) {
        return mColorImpl->getColorBalance();
    }

    return 0;
}

Return<bool> ColorBalance::setValue(int32_t value) {
    if (mColorImpl != nullptr) {
        return mColorImpl->setColorBalance(value);
    }

    return false;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
