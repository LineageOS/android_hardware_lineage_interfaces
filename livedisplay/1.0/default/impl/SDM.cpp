/*
 * Copyright (C) 2016 The CyanogenMod Project
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

#define LOG_TAG "LiveDisplay-SDM"

#include <android-base/properties.h>

#include "Utils.h"
#include "impl/SDM.h"

#define DPPS_BUF_SIZE 64

#define FEATURE_VER_SW_PA_API 0x00000001
#define FEATURE_VER_SW_COLORBAL_API 0x00000003
#define FEATURE_VER_SW_SAVEMODES_API 0x00000004

#define FOSS_ON "foss:on"
#define FOSS_OFF "foss:off"

// For use when only sysfs modes are available
#define STANDARD_NODE_ID 600

#define SRGB_NODE "/sys/class/graphics/fb0/srgb"
#define SRGB_NODE_ID 601

#define DCI_P3_NODE "/sys/class/graphics/fb0/dci_p3"
#define DCI_P3_NODE_ID 602

#define PRIV_MODE_FLAG_SDM 1
#define PRIV_MODE_FLAG_SYSFS 2

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::BAD_VALUE;
using ::android::OK;

status_t SDM::initialize() {
    status_t rc = NO_INIT;

    rc = SDMController::getInstance().init(&mHandle, 0);
    if (rc != OK) {
        return rc;
    }

    mActiveModeId = -1;

    if (hasFeature(Feature::DISPLAY_MODES)) {
        rc = saveInitialDisplayMode();
        if (rc != OK) {
            LOG(ERROR) << "Failed to save initial display mode! err=" << rc;
            return rc;
        }
        sp<disp_mode> defMode = getDefaultDisplayMode();
        if (defMode != nullptr) {
            setDisplayMode(defMode->id, false);
        }
    }

    mFOSSEnabled = stol(android::base::GetProperty("ro.qualcomm.foss", "0")) > 0;

    return OK;
}

status_t SDM::deinitialize() {
    SDMController::getInstance().deinit(mHandle, 0);
    mHandle = -1;
    return OK;
}

uint32_t SDM::getNumSDMDisplayModes() {
    uint32_t flags = 0;
    int32_t count = 0;
    if (SDMController::getInstance().get_num_display_modes(mHandle, 0, 0, &count, &flags)) {
        count = 0;
    }
    return count;
}

uint32_t SDM::getNumDisplayModes() {
    int32_t count = getNumSDMDisplayModes();
    if (getLocalSRGBMode() != nullptr) {
        count++;
    }
    if (getLocalDCIP3Mode() != nullptr) {
        count++;
    }
    return count;
}

sp<disp_mode> SDM::getDisplayModeById(int32_t id) {
    vector<sp<disp_mode>> profiles;
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

status_t SDM::setModeState(sp<disp_mode> mode, bool state) {
    int32_t id = 0;

    if (mode->privFlags == PRIV_MODE_FLAG_SYSFS) {
        if (mode->id != STANDARD_NODE_ID) {
            LOG(VERBOSE) << "sysfs node: " << mode->privData << " state=" << state;
            return Utils::writeInt(mode->privData.c_str(), state ? 1 : 0);
        } else {
            // NOOP
            return OK;
        }
    } else if (mode->privFlags == PRIV_MODE_FLAG_SDM) {
        if (state) {
            return SDMController::getInstance().set_active_display_mode(mHandle, 0, mode->id, 0);
        } else {
            if (Utils::readInitialModeId(&id) == OK) {
                LOG(VERBOSE) << "set sdm mode to default: id" << id;
                return SDMController::getInstance().set_active_display_mode(mHandle, 0, id, 0);
            }
        }
    }
    return BAD_VALUE;
}

status_t SDM::getDisplayModes(vector<sp<disp_mode>>& profiles) {
    status_t rc = OK;

    uint32_t count = getNumDisplayModes();
    if (!count) return rc;

    uint32_t sdm_count = getNumSDMDisplayModes();

    if (sdm_count == 0) {
        // sysfs only case, create a dummy standard mode
        const sp<disp_mode> m = new disp_mode;
        m->id = STANDARD_NODE_ID;
        m->name = "standard";
        m->privFlags = PRIV_MODE_FLAG_SYSFS;
        m->privData = "";
        profiles.push_back(m);
    } else {
        struct sdm_mode {
            int32_t id;
            int32_t type;
            int32_t len;
            char* name;
        };

        sdm_mode* tmp = new sdm_mode[sdm_count];
        memset(tmp, 0, sizeof(sdm_mode) * sdm_count);
        for (uint32_t i = 0; i < sdm_count; i++) {
            tmp[i].id = -1;
            tmp[i].name = new char[128];
            tmp[i].len = 128;
        }

        uint32_t flags = 0;
        rc = SDMController::getInstance().get_display_modes(mHandle, 0, 0, tmp, sdm_count, &flags);
        if (rc == 0) {
            for (uint32_t i = 0; i < sdm_count; i++) {
                const sp<disp_mode> m = new disp_mode;
                m->id = tmp[i].id;
                m->name = tmp[i].name;
                m->privFlags = PRIV_MODE_FLAG_SDM;
                profiles.push_back(m);
            }
        }
        for (uint32_t i = 0; i < sdm_count; i++) delete tmp[i].name;
        delete[] tmp;
    }

    sp<disp_mode> srgb = getLocalSRGBMode();
    sp<disp_mode> dci_p3 = getLocalDCIP3Mode();
    if (srgb != nullptr) {
        profiles.push_back(srgb);
    }
    if (dci_p3 != nullptr) {
        profiles.push_back(dci_p3);
    }

    return rc;
}

sp<disp_mode> SDM::getCurrentDisplayMode() {
    return getDisplayModeById(mActiveModeId);
}

sp<disp_mode> SDM::getDefaultDisplayMode() {
    int32_t id = 0;
    if (Utils::readLocalModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    if (Utils::readInitialModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    return nullptr;
}

sp<disp_mode> SDM::getLocalSRGBMode() {
    char path[PATH_MAX];
    sprintf(path, "%s", SRGB_NODE);

    if (access(path, W_OK) != 0) {
        return nullptr;
    }
    sp<disp_mode> m = new disp_mode;
    m->id = SRGB_NODE_ID;
    m->name = "srgb";
    m->privFlags = PRIV_MODE_FLAG_SYSFS;
    m->privData = path;
    return m;
}

sp<disp_mode> SDM::getLocalDCIP3Mode() {
    char path[PATH_MAX];
    sprintf(path, "%s", DCI_P3_NODE);

    if (access(path, W_OK) != 0) {
        return nullptr;
    }
    sp<disp_mode> m = new disp_mode;
    m->id = DCI_P3_NODE_ID;
    m->name = "dci_p3";
    m->privFlags = PRIV_MODE_FLAG_SYSFS;
    m->privData = path;
    return m;
}

status_t SDM::getPictureAdjustmentRanges(HSICRanges& ranges) {
    hsic_ranges r;
    memset(&r, 0, sizeof(struct hsic_ranges));

    status_t rc = SDMController::getInstance().get_global_pa_range(mHandle, 0, &r);
    if (rc == OK) {
        ranges.hue.min = r.hue.min;
        ranges.hue.max = r.hue.max;
        ranges.hue.step = r.hue.step;
        ranges.saturation.min = r.saturation.min;
        ranges.saturation.max = r.saturation.max;
        ranges.saturation.step = r.saturation.step;
        ranges.intensity.min = r.intensity.min;
        ranges.intensity.max = r.intensity.max;
        ranges.intensity.step = r.intensity.step;
        ranges.contrast.min = r.contrast.min;
        ranges.contrast.max = r.contrast.max;
        ranges.contrast.step = r.contrast.step;
        ranges.saturationThreshold.min = r.saturationThreshold.min;
        ranges.saturationThreshold.max = r.saturationThreshold.max;
        ranges.saturationThreshold.step = r.saturationThreshold.step;
    }
    return rc;
}

bool SDM::hasFeature(Feature feature) {
    uint32_t id = 0, flags = 0;
    struct version {
        uint8_t x, y;
        uint16_t z;
    };
    version v;

    switch (feature) {
        case Feature::DISPLAY_MODES:
            id = FEATURE_VER_SW_SAVEMODES_API;
            break;
        case Feature::COLOR_BALANCE:
            id = FEATURE_VER_SW_COLORBAL_API;
            break;
        case Feature::PICTURE_ADJUSTMENT:
            id = FEATURE_VER_SW_PA_API;
        case Feature::ADAPTIVE_BACKLIGHT:
            if (mFOSSEnabled) {
                return true;
            }
            break;
        default:
            return false;
    }

    if (SDMController::getInstance().get_feature_version(mHandle, id, &v, &flags) != OK) {
        return false;
    }

    if (v.x <= 0 && v.y <= 0 && v.z <= 0) {
        return false;
    }

    // Color balance depends on calibration data in SDM
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
    } else if (feature == Feature::PICTURE_ADJUSTMENT) {
        HSICRanges r;
        if (getPictureAdjustmentRanges(r) == OK && r.isValid()) {
            return true;
        }
    }

    return false;
}

status_t SDM::saveInitialDisplayMode() {
    int32_t id = 0;
    uint32_t flags = 0;
    if (Utils::readInitialModeId(&id) != OK || id < 0) {
        if (SDMController::getInstance().get_default_display_mode(mHandle, 0, &id, &flags) == OK &&
            id >= 0) {
            return Utils::writeInitialModeId(id);
        } else {
            return Utils::writeInitialModeId(0);
        }
    }
    return OK;
}

status_t SDM::getColorBalanceRange(Range& range) {
    status_t rc = SDMController::getInstance().get_global_color_balance_range(mHandle, 0, &range);
    LOG(VERBOSE) << "getColorBalanceRange: min=" << range.min << " max=" << range.max
                 << " step=" << range.step;
    return rc;
}

status_t SDM::getPictureAdjustment(HSIC& hsic) {
    uint32_t enable = 0;
    hsic_config config;
    memset(&config, 0, sizeof(struct hsic_config));

    status_t rc = SDMController::getInstance().get_global_pa_config(mHandle, 0, &enable, &config);
    if (rc == OK) {
        hsic.hue = config.data.hue;
        hsic.saturation = config.data.saturation;
        hsic.intensity = config.data.intensity;
        hsic.contrast = config.data.contrast;
        hsic.saturationThreshold = config.data.saturationThreshold;
    }
    return rc;
}

status_t SDM::setDisplayMode(int32_t modeID, bool makeDefault) {
    status_t rc = OK;

    if (modeID == mActiveModeId) {
        return rc;
    }

    sp<disp_mode> mode = getDisplayModeById(modeID);
    if (mode == nullptr) {
        return BAD_VALUE;
    }

    LOG(DEBUG) << "setDisplayMode: current mode=" << mActiveModeId;

    if (mActiveModeId >= 0) {
        sp<disp_mode> oldMode = getCurrentDisplayMode();
        LOG(DEBUG) << "setDisplayMode: oldMode=" << oldMode->id << " flags=" << oldMode->privFlags;
        if (oldMode->privFlags == PRIV_MODE_FLAG_SYSFS || mode->privFlags == PRIV_MODE_FLAG_SYSFS) {
            LOG(DEBUG) << "disabling old mode";
            rc = setModeState(oldMode, false);
            if (rc != OK) {
                LOG(ERROR) << "Failed to disable previous mode! err=" << rc;
                return rc;
            }
        }
    }

    rc = setModeState(mode, true);
    if (rc == OK) {
        mActiveModeId = mode->id;
        if (makeDefault) {
            rc = Utils::writeLocalModeId(mode->id);
            if (rc != OK) {
                LOG(ERROR) << "failed to save mode! err=" << rc;
                return rc;
            }
            if (mode->privFlags == PRIV_MODE_FLAG_SDM) {
                rc = SDMController::getInstance().set_default_display_mode(mHandle, 0, mode->id, 0);
                if (rc != OK) {
                    LOG(ERROR) << "failed to save mode! err=" << rc;
                    return rc;
                }
            }
        }
        HSIC tmp;
        rc = getPictureAdjustment(tmp);
        if (rc != OK) {
            LOG(ERROR) << "failed to retrieve picture adjustment after mode setting!";
        } else {
            LOG(DEBUG) << "new default PA: " << tmp.hue << " " << tmp.saturation << " "
                       << tmp.intensity << " " << tmp.contrast << " " << tmp.saturationThreshold;
            mDefaultPictureAdjustment = tmp;
        }
    } else {
        LOG(ERROR) << "Failed to setModeState! err=" << rc;
        return rc;
    }

    LOG(DEBUG) << "setDisplayMode: " << modeID << " default: " << makeDefault
               << " flags: " << mode->privFlags;
    return OK;
}

status_t SDM::setAdaptiveBacklightEnabled(bool enabled) {
    status_t rc = NO_INIT;
    if (enabled == mCachedFOSSStatus) {
        return OK;
    }
    char* buf = new char[DPPS_BUF_SIZE];
    sprintf(buf, "%s", enabled ? FOSS_ON : FOSS_OFF);
    if (Utils::sendDPPSCommand(buf, DPPS_BUF_SIZE) == OK) {
        if (strncmp(buf, "Success", 7) == 0) {
            rc = OK;
            mCachedFOSSStatus = enabled;
        }
    }
    delete[] buf;
    return rc;
}

bool SDM::isAdaptiveBacklightEnabled() {
    return mCachedFOSSStatus;
}

int32_t SDM::getColorBalance() {
    int32_t value = -1;
    uint32_t flags = 0;
    if (SDMController::getInstance().get_global_color_balance(mHandle, 0, &value, &flags) != 0) {
        value = 0;
    }
    return value;
}

status_t SDM::setColorBalance(int32_t balance) {
    return SDMController::getInstance().set_global_color_balance(mHandle, 0, balance, 0);
}

status_t SDM::setPictureAdjustment(const HSIC& hsic) {
    hsic_config config;
    memset(&config, 0, sizeof(struct hsic_config));
    config.data.hue = hsic.hue;
    config.data.saturation = hsic.saturation;
    config.data.intensity = hsic.intensity;
    config.data.contrast = hsic.contrast;
    config.data.saturationThreshold = hsic.saturationThreshold;

    return SDMController::getInstance().set_global_pa_config(mHandle, 0, 1, &config);
}

HSIC SDM::getDefaultPictureAdjustment() {
    return mDefaultPictureAdjustment;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
