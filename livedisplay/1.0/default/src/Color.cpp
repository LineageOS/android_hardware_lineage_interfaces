/*
 * Copyright (C) 2017-2018 The LineageOS Project
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

// #define LOG_NDEBUG 0

#define LOG_TAG "LiveDisplay-HIDL"

#include "Color.h"

#ifdef COLOR_BACKEND_SDM
#include "impl/SDM.h"
#else
#error "Color backend undefined!"
#endif

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::NO_INIT;
using ::android::OK;

sp<Color> Color::sInstance = nullptr;

Color::Color() : mConnected(false), mBackend(nullptr) {
#ifdef COLOR_BACKEND_SDM
    mBackend = new SDM();
#endif
    LOG(DEBUG) << "Loaded LiveDisplay native interface";
}

Color::~Color() {
    reset();
}

void Color::reset() {
    if (mConnected) {
        mBackend->deinitialize();
    }
    mFeatures = 0;
    mConnected = false;
}

void Color::error(const char* msg) {
    if (msg != NULL) {
        LOG(ERROR) << msg;
    }

    reset();
}

bool Color::connect() {
    if (mConnected) {
        return true;
    }

    mFeatures = 0;

    if (mBackend == NULL) {
        return false;
    }

    if (mBackend->initialize() != OK) {
        LOG(ERROR) << "Failed to initialize backend!";
        return false;
    }

    for (uint32_t i = 1; i <= (uint32_t)Feature::MAX; i <<= 1) {
        Feature f = static_cast<Feature>(i);
        if (mBackend->hasFeature(f)) {
            addFeature(f);
        }
    }
    mConnected = true;

    return mFeatures > 0;
}

sp<Color> Color::getInstance() {
    if (sInstance == nullptr) {
        sInstance = new Color();
    }
    return sInstance;
}

DisplayMode Color::modePointerToObj(sp<disp_mode> mode) {
    DisplayMode m;
    m.id = mode->id;
    m.name = mode->name;
    return m;
}

DisplayMode Color::invalidDisplayMode() {
    DisplayMode mode;
    mode.id = -1;
    return mode;
}

Return<Features> Color::getSupportedFeatures() {
    connect();
    return mFeatures;
}

Return<void> Color::getDisplayModes(getDisplayModes_cb _hidl_cb) {
    hidl_vec<DisplayMode> profiles;
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        vector<sp<disp_mode>> spProfiles;
        rc = mBackend->getDisplayModes(spProfiles);
        if (rc != OK) {
            error("Unable to fetch display modes!");
        } else {
            profiles.resize(spProfiles.size());
            for (size_t i = 0; i < spProfiles.size(); i++) {
                profiles[i].id = spProfiles[i]->id;
                profiles[i].name = spProfiles[i]->name;
            }
        }
    }

    _hidl_cb(profiles);
    return Void();
}

Return<void> Color::getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) {
    DisplayMode mode;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        sp<disp_mode> m = mBackend->getCurrentDisplayMode();
        if (m != nullptr) {
            mode = modePointerToObj(m);
        } else {
            mode = invalidDisplayMode();
        }
    }
    _hidl_cb(mode);
    return Void();
}

Return<void> Color::getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) {
    DisplayMode mode;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        sp<disp_mode> m = mBackend->getDefaultDisplayMode();
        if (m != nullptr) {
            mode = modePointerToObj(m);
        } else {
            mode = invalidDisplayMode();
        }
    }
    _hidl_cb(mode);
    return Void();
}

Return<bool> Color::setDisplayMode(int32_t modeID, bool makeDefault) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        rc = mBackend->setDisplayMode(modeID, makeDefault);
        if (rc != OK) {
            error("Unable to set display mode!");
        }
    }
    return rc == OK;
}

Return<bool> Color::setAdaptiveBacklightEnabled(bool enabled) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
        rc = mBackend->setAdaptiveBacklightEnabled(enabled);
        if (rc != OK) {
            error("Unable to set adaptive backlight state!");
        }
    }
    return rc == OK;
}

Return<bool> Color::isAdaptiveBacklightEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
        return mBackend->isAdaptiveBacklightEnabled();
    }
    return false;
}

Return<bool> Color::setOutdoorModeEnabled(bool enabled) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::OUTDOOR_MODE)) {
        rc = mBackend->setOutdoorModeEnabled(enabled);
        if (rc != OK) {
            error("Unable to toggle outdoor mode!");
        }
    }
    return rc == OK;
}

Return<bool> Color::isOutdoorModeEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::OUTDOOR_MODE)) {
        return mBackend->isOutdoorModeEnabled();
    }
    return false;
}

Return<void> Color::getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) {
    Range range;
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_BALANCE)) {
        status_t rc = mBackend->getColorBalanceRange(range);
        if (rc != OK) {
            error("Unable to fetch color balance range!");
            range.max = range.min = 0;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<int32_t> Color::getColorBalance() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_BALANCE)) {
        return mBackend->getColorBalance();
    }

    return 0;
}

Return<bool> Color::setColorBalance(int32_t value) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_BALANCE)) {
        rc = mBackend->setColorBalance(value);
        if (rc != OK) {
            error("Unable to set color balance!");
        }
    }
    return rc == OK;
}

Return<bool> Color::setPictureAdjustment(const HSIC& hsic) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        rc = mBackend->setPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to set picture adjustment!");
        }
    }
    return rc == OK;
}

Return<void> Color::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    HSIC hsic;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to get picture adjustment!");
        }
    }
    _hidl_cb(hsic);
    return Void();
}

Return<void> Color::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
    HSIC hsic;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        hsic = mBackend->getDefaultPictureAdjustment();
    }
    _hidl_cb(hsic);
    return Void();
}

Return<void> Color::getHueRange(getHueRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get hue range!");
        }
    }
    _hidl_cb(ranges.hue);
    return Void();
}

Return<void> Color::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get saturation range!");
        }
    }
    _hidl_cb(ranges.saturation);
    return Void();
}

Return<void> Color::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get intensity range!");
        }
    }
    _hidl_cb(ranges.intensity);
    return Void();
}

Return<void> Color::getContrastRange(getContrastRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get contrast range!");
        }
    }
    _hidl_cb(ranges.contrast);
    return Void();
}

Return<void> Color::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get saturation threshold range!");
        }
    }
    _hidl_cb(ranges.saturationThreshold);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
