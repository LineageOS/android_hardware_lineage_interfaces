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

#include "DisplayModes.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

DisplayModes::DisplayModes(sp<Color> colorImpl) {
    mColorImpl = colorImpl;
}

bool DisplayModes::isSupported() {
    if (mColorImpl != nullptr) {
        return mColorImpl->check(Feature::DISPLAY_MODES);
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModes::getModes(getModes_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getDisplayModes(_hidl_cb);
    } else {
        _hidl_cb({});
    }

    return Void();
}

Return<void> DisplayModes::getCurrentMode(getCurrentMode_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getCurrentDisplayMode(_hidl_cb);
    } else {
        DisplayMode mode;
        mode.id = -1;
        _hidl_cb(mode);
    }

    return Void();
}

Return<void> DisplayModes::getDefaultMode(getDefaultMode_cb _hidl_cb) {
    if (mColorImpl != nullptr) {
        mColorImpl->getDefaultDisplayMode(_hidl_cb);
    } else {
        DisplayMode mode;
        mode.id = -1;
        _hidl_cb(mode);
    }

    return Void();
}

Return<bool> DisplayModes::setMode(int32_t modeID, bool makeDefault) {
    if (mColorImpl != nullptr) {
        return mColorImpl->setDisplayMode(modeID, makeDefault);
    }

    return false;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
