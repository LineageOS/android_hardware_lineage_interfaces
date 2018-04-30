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

#include "impl/LegacyMM.h"

#include "Utils.h"

#include <android-base/logging.h>

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
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using android::BAD_VALUE;
using android::NO_INIT;
using android::OK;
using android::sp;
using android::status_t;

status_t LegacyMM::getColorBalanceRange(Range& range) {
    struct mm_range r;
    memset(&r, 0, sizeof(struct mm_range));

    status_t rc = LegacyMMController::getInstance().get_color_balance_range(0, &r);
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

    uint32_t count = getNumDisplayModes();

    if (!count) {
        return rc;
    }

    struct d_mode {
        int id;
        char* name;
        uint32_t len;
        int32_t type;
        d_mode() : id(-1), len(128), type(0) {
            name = new char[128];
        }
        ~d_mode() {
            delete name;
        }
    };

    d_mode tmp[count];

    rc = LegacyMMController::getInstance().get_display_modes(0, 0, tmp, count);
    if (rc == 0) {
        for (uint32_t i = 0; i < count; i++) {
            const sp<disp_mode> m = new disp_mode;
            m->id = tmp[i].id;
            m->name = tmp[i].name;
            m->privFlags = 0;  // TODO: fix flag
            profiles.push_back(m);
        }
    }

    return rc;
}

status_t LegacyMM::setDisplayMode(int32_t modeID, bool makeDefault) {
    auto currentMode = getCurrentDisplayMode();
    if (currentMode != nullptr && currentMode->id == modeID) {
        return OK;
    }

    auto mode = getDisplayModeById(modeID);
    if (mode == nullptr) {
        return BAD_VALUE;
    }

    if (LegacyMMController::getInstance().set_active_display_mode(0, modeID) != 0) {
        return BAD_VALUE;
    }

    if (makeDefault && LegacyMMController::getInstance().set_default_display_mode(0, modeID) != 0) {
        return BAD_VALUE;
    }

    HSIC tmp;
    if (getPictureAdjustment(tmp) == OK) {
        mDefaultPictureAdjustment = tmp;
    }

    return OK;
}

sp<disp_mode> LegacyMM::getCurrentDisplayMode() {
    int32_t id = 0;
    uint32_t mask = 0;

    status_t rc = LegacyMMController::getInstance().get_active_display_mode(0, &id, &mask);
    if (rc == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return nullptr;
}

sp<disp_mode> LegacyMM::getDefaultDisplayMode() {
    int32_t id = 0;

    if (Utils::readLocalModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    if (Utils::readInitialModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    status_t rc = LegacyMMController::getInstance().get_default_display_mode(0, &id);
    if (rc == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return nullptr;
}

status_t LegacyMM::getPictureAdjustmentRanges(HSICRanges& ranges) {
    struct mm_pa_range r;
    memset(&r, 0, sizeof(struct mm_pa_range));

    status_t rc = LegacyMMController::getInstance().get_pa_range(0, &r);
    if (rc == OK) {
        ranges.hue.min = r.min.hue;
        ranges.hue.max = r.max.hue;
        ranges.hue.step = 1;
        ranges.saturation.min = r.min.saturation;
        ranges.saturation.max = r.max.saturation;
        ranges.saturation.step = 1;
        ranges.intensity.min = r.min.intensity;
        ranges.intensity.max = r.max.intensity;
        ranges.intensity.step = 1;
        ranges.contrast.min = r.min.contrast;
        ranges.contrast.max = r.max.contrast;
        ranges.contrast.step = 1;
        ranges.saturationThreshold.min = r.min.saturationThreshold;
        ranges.saturationThreshold.max = r.max.saturationThreshold;
        ranges.saturationThreshold.step = 1;
    }
    return rc;
}

status_t LegacyMM::getPictureAdjustment(HSIC& hsic) {
    struct mm_pa_config config;
    memset(&config, 0, sizeof(struct mm_pa_config));

    status_t rc = LegacyMMController::getInstance().get_pa_config(0, &config);
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
    return mDefaultPictureAdjustment;
}

status_t LegacyMM::setPictureAdjustment(const HSIC& hsic) {
    struct mm_pa_config config;
    memset(&config, 0, sizeof(struct mm_pa_config));

    config.flags = 0x0F;  // lower 4 bits
    config.data.hue = hsic.hue;
    config.data.saturation = hsic.saturation;
    config.data.intensity = hsic.intensity;
    config.data.contrast = hsic.contrast;
    config.data.saturationThreshold = hsic.saturationThreshold;

    return LegacyMMController::getInstance().set_pa_config(0, &config);
}

status_t LegacyMM::initialize() {
    status_t rc = NO_INIT;
    rc = LegacyMMController::getInstance().init(0);
    if (rc != OK) {
        LOG(ERROR) << "%s: Failed to initialize LegacyMMController";
        return rc;
    }

    if (hasFeature(Feature::DISPLAY_MODES)) {
        int32_t id;
        // Get the initial mode from Utils
        rc = Utils::readInitialModeId(&id);
        if (rc != OK || id < 0) {
            // Get controller default mode and save it
            rc = LegacyMMController::getInstance().get_default_display_mode(0, &id);
            if (rc == OK && id >= 0) {
                Utils::writeInitialModeId(id);
            } else {
                Utils::writeInitialModeId(0);
            }
        }

        auto mode = getDefaultDisplayMode();
        if (mode != nullptr) {
            setDisplayMode(mode->id, false);
        }
    }
    return OK;
}

status_t LegacyMM::deinitialize() {
    status_t rc = NO_INIT;
    rc = LegacyMMController::getInstance().init(1);
    if (rc != OK) {
        return rc;
    }
    return OK;
}

bool LegacyMM::hasFeature(Feature feature) {
    uint32_t id;
    switch (feature) {
        case Feature::COLOR_BALANCE:
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
        if (feature == Feature::DISPLAY_MODES || feature == Feature::COLOR_BALANCE) {
            if (getNumDisplayModes() > 0) {
                // make sure the range isn't zero
                if (feature == Feature::COLOR_BALANCE) {
                    Range r;
                    if (getColorBalanceRange(r) == OK && isNonZero(r)) {
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

uint32_t LegacyMM::getNumDisplayModes() {
    uint32_t count = 0;
    if (LegacyMMController::getInstance().get_num_display_modes(0, 0, &count) != 0) {
        count = 0;
    }
    return count;
}

sp<disp_mode> LegacyMM::getDisplayModeById(int32_t id) {
    std::vector<sp<disp_mode>> profiles;
    status_t rc = getDisplayModes(profiles);
    if (rc == OK) {
        for (const auto& mode : profiles) {
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
