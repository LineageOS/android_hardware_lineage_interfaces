/*
** Copyright 2016, The CyanogenMod Project
** Copyright (C) 2017-2018 The LineageOS Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

// #define LOG_NDEBUG 0

#define LOG_TAG "LiveDisplay-LegacyMM"

#include <android-base/properties.h>

#include "Utils.h"
#include "impl/LegacyMM.h"

namespace {
struct mm_pa_data {
    int hue;
    int saturation;
    int intensity;
    int contrast;
    int saturationThreshold;
};

struct mm_pa_config {
    int flags;
    struct mm_pa_data data;
};

struct mm_pa_range {
    struct mm_pa_data max;
    struct mm_pa_data min;
};

struct mm_range {
    int max;
    int min;
};
} // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

status_t LegacyMM::getColorBalanceRange(Range& range) {
    struct mm_range r;
    memset(&r, 0, sizeof(struct mm_range));

    int32_t rc = LegacyMMController::getInstance().get_color_balance_range(0, &r);
    if (rc == OK) {
        range.min = r.min;
        range.max = r.max;
    }
    return rc;
}

status_t LegacyMM::setColorBalance(int32_t balance) {
    return LegacyMMController::getInstance().set_color_balance(0, balance);
}

int32_t LegacyMM::getColorBalance() {
    int32_t value = 0;
    if (LegacyMMController::getInstance().get_color_balance(0, &value) != 0) {
        value = 0;
    }
    return value;
}

status_t LegacyMM::getDisplayModes(std::vector<sp<disp_mode>>& profiles) {
    status_t rc = OK;
    int i = 0;

    int count = getNumDisplayModes();

    if (!count) return rc;

    struct d_mode {
        int id;
        char* name;
        uint32_t len;
        int32_t type;
    };

    d_mode* tmp = new d_mode[count];
    memset(tmp, 0, sizeof(d_mode) * count);
    for (i = 0; i < count; i++) {
        tmp[i].id = -1;
        tmp[i].name = new char[128];
        tmp[i].len = 128;
    }

    rc = disp_api_get_display_modes(0, 0, tmp, count);
    if (rc == 0) {
        for (i = 0; i < count; i++) {
            const sp<DisplayMode> m = new DisplayMode(tmp[i].id, tmp[i].name, tmp[i].len);
            profiles.push_back(m);
            delete tmp[i].name;
        }
    }
    delete[] tmp;

    return rc;
}

status_t LegacyMM::setDisplayMode(int32_t modeID, bool makeDefault) {
    if (disp_api_set_active_display_mode(0, modeID) != 0) {
        return BAD_VALUE;
    }

    if (makeDefault && disp_api_set_default_display_mode(0, modeID) != 0) {
        return BAD_VALUE;
    }

    return OK;
}

sp<disp_mode> LegacyMM::getCurrentDisplayMode() {
    int id = 0;
    uint32_t mask = 0;

    status_t rc = disp_api_get_active_display_mode(0, &id, &mask);
    if (rc == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return nullptr;
}

sp<disp_mode> LegacyMM::getDefaultDisplayMode() {
    int id = 0;

    status_t rc = disp_api_get_default_display_mode(0, &id);
    if (rc == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return nullptr;
}

status_t LegacyMM::getPictureAdjustmentRanges(HSICRanges& ranges) {
    struct mm_pa_range r;
    memset(&r, 0, sizeof(struct mm_pa_range));

    status_t rc = disp_api_get_pa_range(0, &r);
    if (rc == OK) {
        ranges.hue.min = r.min.hue;
        ranges.hue.max = r.max.hue;
        ranges.saturation.min = r.min.saturation;
        ranges.saturation.max = r.max.saturation;
        ranges.intensity.min = r.min.intensity;
        ranges.intensity.max = r.max.intensity;
        ranges.contrast.min = r.min.contrast;
        ranges.contrast.max = r.max.contrast;
        ranges.saturationThreshold.min = r.min.saturationThreshold;
        ranges.saturationThreshold.max = r.max.saturationThreshold;
    }
    return rc;
}

status_t LegacyMM::getPictureAdjustment(HSIC& hsic) {
    struct mm_pa_config config;
    memset(&config, 0, sizeof(struct mm_pa_config));

    status_t rc = disp_api_get_pa_config(0, &config);
    if (rc == OK) {
        hsic.hue = config.data.hue;
        hsic.saturation = config.data.saturation;
        hsic.intensity = config.data.intensity;
        hsic.contrast = config.data.contrast;
        hsic.saturationThreshold = config.data.saturationThreshold;
    }
    return rc;
}

HSIC LegacyMM::getDefaultPictureAdjustment() {
    // TODO: implement
    return nullptr;
}

status_t LegacyMM::setPictureAdjustment(const HSIC& hsic) {
    struct mm_pa_config config;
    memset(&config, 0, sizeof(struct mm_pa_config));

    config.flags = 0x0F;   // lower 4 bits
    config.data.hue = hsic.hue;
    config.data.saturation = hsic.saturation;
    config.data.intensity = hsic.intensity;
    config.data.contrast = hsic.contrast;
    config.data.saturationThreshold = hsic.saturationThreshold;

    return disp_api_set_pa_config(0, &config);
}

status_t LegacyMM::initialize() {
    // TODO: implement
}

status_t LegacyMM::deinitialize() {
    // TODO: implement
}

bool LegacyMM::hasFeature(Feature feature) {
    int id;
    switch (feature) {
        case Feature::COLOR_TEMPERATURE:
            id = 0;
            break;
        case Feature::DISPLAY_MODES:
            id = 1;
            break;
        case Feature::PICTURE_ADJUSTMENT:
            id = 4;
            break;
        default:
            return false;
    }

    if (LegacyMMController::getInstance().supported(0, id)) {
        // display modes and color balance depend on each other
        if (feature == Feature::DISPLAY_MODES ||
                feature == Feature::COLOR_TEMPERATURE) {
            if (getNumDisplayModes() > 0) {
                // make sure the range isn't zero
                if (feature == Feature::COLOR_TEMPERATURE) {
                    Range r;
                    if (getColorBalanceRange(r) == OK && r.isNonZero()) {
                        return true;
                    }
                    return false;
                }
                return true;
            }
        }
        if (feature == Feature::PICTURE_ADJUSTMENT) {
            HSICRanges r;
            if (getPictureAdjustmentRanges(r) == OK && r.isValid()) {
                return true;
            }
        }
    }
    return false;
}

int LegacyMM::getNumDisplayModes() {
    int count = 0;
    if (disp_api_get_num_display_modes(0, 0, &count) != 0) {
        count = 0;
    }
    return count;
}

sp<DisplayMode> LegacyMM::getDisplayModeById(int id) {
    List<sp<DisplayMode>> profiles;
    status_t rc = getDisplayModes(profiles);
    if (rc == OK) {
        for (List<sp<DisplayMode>>::iterator it = profiles.begin(); it != profiles.end(); ++it) {
            const sp<DisplayMode> mode = *it;
            if (id == mode->id) {
                return mode;
            }
        }
    }

    return nullptr;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
