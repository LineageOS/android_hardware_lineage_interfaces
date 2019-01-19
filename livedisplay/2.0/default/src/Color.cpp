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

#include "ColorBackend.h"
#if defined(COLOR_BACKEND_SDM)
#include "impl/SDM.h"
#elif defined(COLOR_BACKEND_LEGACYMM)
#include "impl/LegacyMM.h"
#endif

#include <android-base/logging.h>

namespace {

using vendor::lineage::livedisplay::V2_0::DisplayMode;
using vendor::lineage::livedisplay::V2_0::implementation::disp_mode;

DisplayMode modePointerToObj(android::sp<disp_mode> mode) {
    DisplayMode m;
    m.id = mode->id;
    m.name = mode->name;
    return m;
}

DisplayMode invalidDisplayMode() {
    DisplayMode mode;
    mode.id = -1;
    return mode;
}
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

using ::android::Mutex;
using ::android::NO_INIT;
using ::android::OK;
using ::android::sp;
using ::android::status_t;

Color::Color() : mConnected(false), mBackend(nullptr) {
}

Color::~Color() {
    reset();
}

void Color::reset() {
    if (mConnected) {
        mBackend = nullptr;
    }
    mFeatures = 0;
    mConnected = false;
}

bool Color::check(Feature f) {
    return connect() && (mFeatures & (uint32_t)f);
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

#if defined(COLOR_BACKEND_SDM)
    mBackend.reset(new SDM());
#elif defined(COLOR_BACKEND_LEGACYMM)
    mBackend.reset(new LegacyMM());
#endif
    if (mBackend == nullptr) {
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

uint32_t Color::getSupportedFeatures() {
    connect();
    return mFeatures;
}

// IAdaptiveBacklight
bool Color::setAdaptiveBacklightEnabled(bool enabled) {
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

bool Color::isAdaptiveBacklightEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
        return mBackend->isAdaptiveBacklightEnabled();
    }
    return false;
}

// IColorBalance
void Color::getColorBalanceRange(IColorBalance::getRange_cb _hidl_cb) {
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
}

int32_t Color::getColorBalance() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_BALANCE)) {
        return mBackend->getColorBalance();
    }

    return 0;
}

bool Color::setColorBalance(int32_t value) {
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

// IDisplayModes
void Color::getDisplayModes(IDisplayModes::getModes_cb _hidl_cb) {
    hidl_vec<DisplayMode> profiles;
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        std::vector<sp<disp_mode>> spProfiles;
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
}

void Color::getCurrentDisplayMode(IDisplayModes::getCurrentMode_cb _hidl_cb) {
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
}

void Color::getDefaultDisplayMode(IDisplayModes::getDefaultMode_cb _hidl_cb) {
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
}

bool Color::setDisplayMode(int32_t modeID, bool makeDefault) {
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

// ISunlightEnhancement
bool Color::setOutdoorModeEnabled(bool enabled) {
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

bool Color::isOutdoorModeEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::OUTDOOR_MODE)) {
        return mBackend->isOutdoorModeEnabled();
    }
    return false;
}

// IPictureAdjustment
bool Color::setPictureAdjustment(const HSIC& hsic) {
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

void Color::getPictureAdjustment(IPictureAdjustment::getAdjustment_cb _hidl_cb) {
    HSIC hsic;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to get picture adjustment!");
        }
    }
    _hidl_cb(hsic);
}

void Color::getDefaultPictureAdjustment(IPictureAdjustment::getDefaultAdjustment_cb _hidl_cb) {
    HSIC hsic;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        hsic = mBackend->getDefaultPictureAdjustment();
    }
    _hidl_cb(hsic);
}

void Color::getHueRange(IPictureAdjustment::getHueRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get hue range!");
        }
    }
    _hidl_cb(ranges.hue);
}

void Color::getSaturationRange(IPictureAdjustment::getSaturationRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get saturation range!");
        }
    }
    _hidl_cb(ranges.saturation);
}

void Color::getIntensityRange(IPictureAdjustment::getIntensityRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get intensity range!");
        }
    }
    _hidl_cb(ranges.intensity);
}

void Color::getContrastRange(IPictureAdjustment::getContrastRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get contrast range!");
        }
    }
    _hidl_cb(ranges.contrast);
}

void Color::getSaturationThresholdRange(IPictureAdjustment::getSaturationThresholdRange_cb _hidl_cb) {
    HSICRanges ranges;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get saturation threshold range!");
        }
    }
    _hidl_cb(ranges.saturationThreshold);
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
