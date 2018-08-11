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

ColorBalance::ColorBalance() {
    mColorHal = IColor::getService();
    if (mColorHal == nullptr) {
        return nullptr;
    }
}

// Methods from IColorBalance follow.
Return<void> ColorBalance::getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) {
    return mColorHal->getColorBalanceRange(_hidl_cb);
}

Return<int32_t> ColorBalance::getColorBalance() {
    return mColorHal->getColorBalance();
}

Return<bool> ColorBalance::setColorBalance(int32_t value) {
    return mColorHal->setColorBalance(value);
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
