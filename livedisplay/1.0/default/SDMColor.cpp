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

#define LOG_NDEBUG 0

#define LOG_TAG "LiveDisplay-HIDL"

#include <cutils/properties.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <log/log.h>

#include "SDMColor.h"
#include "Utils.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Void;
using ::android::Utils;

status_t SDMColor::loadVendorLibrary() {
    if (mLibHandle != NULL) {
        return OK;
    }

    mLibHandle = dlopen(SDM_DISP_LIB, RTLD_NOW);
    if (mLibHandle == NULL) {
        ALOGE("DLOPEN failed for %s (%s)", SDM_DISP_LIB, dlerror());
        ALOGE("Attempting to DLOPEN legacy lib %s", SDM_DISP_LEGACY_LIB);
        mLibHandle = dlopen(SDM_DISP_LEGACY_LIB, RTLD_NOW);
        if (mLibHandle == NULL) {
            ALOGE("DLOPEN failed for legacy lib %s (%s)", SDM_DISP_LEGACY_LIB, dlerror());
            return NO_INIT;
        } else {
            ALOGE("%s successfully DLOPENed. This is deprecated! Switch to new %s",
                   SDM_DISP_LEGACY_LIB, SDM_DISP_LIB);
        }
    }

    disp_api_init = (int32_t(*)(int64_t*, uint32_t))dlsym(mLibHandle, "disp_api_init");
    if (disp_api_init == NULL) {
        ALOGE("dlsym failed for disp_api_init");
        goto fail;
    }
    disp_api_deinit = (int32_t(*)(int64_t, uint32_t))dlsym(mLibHandle, "disp_api_deinit");
    if (disp_api_deinit == NULL) {
        ALOGE("dlsym failed for disp_api_deinit");
        goto fail;
    }
    disp_api_get_global_color_balance_range = (int32_t(*)(int64_t, uint32_t, void*))dlsym(
        mLibHandle, "disp_api_get_global_color_balance_range");
    if (disp_api_get_global_color_balance_range == NULL) {
        ALOGE("dlsym failed for disp_api_get_global_color_balance_range");
        goto fail;
    }
    disp_api_set_global_color_balance = (int32_t(*)(int64_t, uint32_t, int32_t, uint32_t))dlsym(
        mLibHandle, "disp_api_set_global_color_balance");
    if (disp_api_set_global_color_balance == NULL) {
        ALOGE("dlsym failed for disp_api_set_global_color_balance");
        goto fail;
    }
    disp_api_get_global_color_balance = (int32_t(*)(int64_t, uint32_t, int32_t*, uint32_t*))dlsym(
        mLibHandle, "disp_api_get_global_color_balance");
    if (disp_api_get_global_color_balance == NULL) {
        ALOGE("dlsym failed for disp_api_get_global_color_balance");
        goto fail;
    }
    disp_api_get_num_display_modes =
        (int32_t(*)(int64_t, uint32_t, int32_t, int32_t*, uint32_t*))dlsym(
            mLibHandle, "disp_api_get_num_display_modes");
    if (disp_api_get_num_display_modes == NULL) {
        ALOGE("dlsym failed for disp_api_get_num_display_modes");
        goto fail;
    }
    disp_api_get_display_modes =
        (int32_t(*)(int64_t, uint32_t, int32_t, void*, int32_t, uint32_t*))dlsym(
            mLibHandle, "disp_api_get_display_modes");
    if (disp_api_get_display_modes == NULL) {
        ALOGE("dlsym failed for disp_api_get_display_modes");
        goto fail;
    }
    disp_api_get_active_display_mode =
        (int32_t(*)(int64_t, uint32_t, int32_t*, uint32_t*, uint32_t*))dlsym(
            mLibHandle, "disp_api_get_active_display_mode");
    if (disp_api_get_active_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_get_active_display_mode");
        goto fail;
    }
    disp_api_set_active_display_mode = (int32_t(*)(int64_t, uint32_t, int32_t, uint32_t))dlsym(
        mLibHandle, "disp_api_set_active_display_mode");
    if (disp_api_set_active_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_set_active_display_mode");
        goto fail;
    }
    disp_api_set_default_display_mode = (int32_t(*)(int64_t, uint32_t, int32_t, uint32_t))dlsym(
        mLibHandle, "disp_api_set_default_display_mode");
    if (disp_api_set_default_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_set_default_display_mode");
        goto fail;
    }
    disp_api_get_default_display_mode = (int32_t(*)(int64_t, uint32_t, int32_t*, uint32_t*))dlsym(
        mLibHandle, "disp_api_get_default_display_mode");
    if (disp_api_get_default_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_get_default_display_mode");
        goto fail;
    }
    disp_api_get_global_pa_range =
        (int32_t(*)(int64_t, uint32_t, void*))dlsym(mLibHandle, "disp_api_get_global_pa_range");
    if (disp_api_get_global_pa_range == NULL) {
        ALOGE("dlsym failed for disp_api_get_global_pa_range");
        goto fail;
    }
    disp_api_get_global_pa_config = (int32_t(*)(int64_t, uint32_t, uint32_t*, void*))dlsym(
        mLibHandle, "disp_api_get_global_pa_config");
    if (disp_api_get_global_pa_config == NULL) {
        ALOGE("dlsym failed for disp_api_get_global_pa_config");
        goto fail;
    }
    disp_api_set_global_pa_config = (int32_t(*)(int64_t, uint32_t, uint32_t, void*))dlsym(
        mLibHandle, "disp_api_set_global_pa_config");
    if (disp_api_set_global_pa_config == NULL) {
        ALOGE("dlsym failed for disp_api_set_global_pa_config");
        goto fail;
    }
    disp_api_get_feature_version = (int32_t(*)(int64_t, uint32_t, void*, uint32_t*))dlsym(
        mLibHandle, "disp_api_get_feature_version");
    if (disp_api_get_feature_version == NULL) {
        ALOGE("dlsym failed for disp_api_get_feature_version");
        goto fail;
    }

    return OK;

fail:
    ALOGE("Failed to link vendor library: %s", dlerror());
    dlclose(mLibHandle);
    mLibHandle = NULL;
    return NO_INIT;
}

status_t SDMColor::initialize() {
    status_t rc = loadVendorLibrary();
    if (rc != OK) {
        return rc;
    }

    rc = disp_api_init(&mHandle, 0);
    if (rc != OK) {
        return rc;
    }

    mActiveModeId = -1;

    if (backendHasFeature(Feature::DISPLAY_MODES)) {
        rc = saveInitialDisplayMode();
        if (rc != OK) {
            ALOGE("Failed to save initial display mode! err=%d", rc);
            return rc;
        }
        sp<disp_mode> defMode = getDefaultDisplayMode();
        if (defMode != nullptr) {
            native_setDisplayMode(defMode->id, false);
        }
    }
    return OK;
}

SDMColor::~SDMColor() {
    if (mLibHandle != NULL) {
        dlclose(mLibHandle);
    }
}

status_t SDMColor::deinitialize() {
    if (mLibHandle != NULL) {
        disp_api_deinit(mHandle, 0);
        mHandle = -1;
    }
    return OK;
}

uint32_t SDMColor::getNumSDMDisplayModes() {
    uint32_t flags = 0;
    int32_t count = 0;
    if (disp_api_get_num_display_modes(mHandle, 0, 0, &count, &flags)) {
        count = 0;
    }
    return count;
}

uint32_t SDMColor::getNumDisplayModes() {
    int32_t count = getNumSDMDisplayModes();
    if (getLocalSRGBMode() != nullptr) {
        count++;
    }
    if (getLocalDCIP3Mode() != nullptr) {
        count++;
    }
    return count;
}

sp<disp_mode> SDMColor::getDisplayModeById(int32_t id) {
    vector<sp<disp_mode>> profiles;
    status_t rc = getDisplayModes(profiles);
    if (rc == OK) {
        for (const auto &mode : profiles) {
            if (id == mode->id) {
                return mode;
            }
        }
    }

    return nullptr;
}

status_t SDMColor::setModeState(sp<disp_mode> mode, bool state) {
    int32_t id = 0;

    if (mode->privFlags == PRIV_MODE_FLAG_SYSFS) {
        if (mode->id != STANDARD_NODE_ID) {
           ALOGV("sysfs node: %s state=%d", mode->privData.c_str(), state);
           return Utils::writeInt(mode->privData.c_str(), state ? 1 : 0);
        } else {
           // NOOP
           return OK;
        }
    } else if (mode->privFlags == PRIV_MODE_FLAG_SDM) {
        if (state) {
            return disp_api_set_active_display_mode(mHandle, 0, mode->id, 0);
        } else {
            if (Utils::readInitialModeId(&id) == OK) {
                ALOGV("set sdm mode to default: id=%d", id);
                return disp_api_set_active_display_mode(mHandle, 0, id, 0);
            }
        }
    }
    return BAD_VALUE;
}

status_t SDMColor::getDisplayModes(vector<sp<disp_mode>>& profiles) {
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
        rc = disp_api_get_display_modes(mHandle, 0, 0, tmp, sdm_count, &flags);
        if (rc == 0) {
            for (uint32_t i = 0; i < sdm_count; i++) {
                const sp<disp_mode> m = new disp_mode;
                m->id = tmp[i].id;
                m->name = string(tmp[i].name, tmp[i].len);
                m->privFlags = PRIV_MODE_FLAG_SDM;
                profiles.push_back(m);
            }
        }
        for (uint32_t i = 0; i < sdm_count; i++)
            delete tmp[i].name;
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

DisplayMode SDMColor::modePointerToObj(sp<disp_mode> mode) {
    DisplayMode m;
    m.id = mode->id;
    m.name = mode->name;
    return m;
}

sp<disp_mode> SDMColor::getCurrentDisplayMode() {
    return getDisplayModeById(mActiveModeId);
}

sp<disp_mode> SDMColor::getDefaultDisplayMode() {
    int32_t id = 0;
    if (Utils::readLocalModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    if (Utils::readInitialModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    return nullptr;
}

sp<disp_mode> SDMColor::getLocalSRGBMode() {
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

sp<disp_mode> SDMColor::getLocalDCIP3Mode() {
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

status_t SDMColor::getPictureAdjustmentRanges(HSICRanges& ranges) {
    hsic_ranges r;
    memset(&r, 0, sizeof(struct hsic_ranges));

    status_t rc = disp_api_get_global_pa_range(mHandle, 0, &r);
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

bool SDMColor::backendHasFeature(Feature feature) {
    uint32_t id = 0, flags = 0;
    struct version {
        uint8_t x, y;
        uint16_t z;
    };
    version v;

    switch (feature) {
        case Feature::DISPLAY_MODES:
            id = 4;
            break;
        case Feature::COLOR_TEMPERATURE:
            id = 3;
            break;
        case Feature::PICTURE_ADJUSTMENT:
            id = 1;
        case Feature::ADAPTIVE_BACKLIGHT:
            if (property_get_int32("ro.qualcomm.foss", 0) > 0) {
                return true;
            }
            break;
        default:
            return false;
    }

    if (disp_api_get_feature_version(mHandle, id, &v, &flags) == 0) {
        if (v.x > 0 || v.y > 0 || v.z > 0) {

            // Color balance depends on calibration data in SDM
            if (feature == Feature::DISPLAY_MODES ||
                    feature == Feature::COLOR_TEMPERATURE) {
                if (getNumDisplayModes() > 0) {
                    // make sure the range isn't zero
                    if (feature == Feature::COLOR_TEMPERATURE) {
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
        }
    }
    return false;
}

status_t SDMColor::saveInitialDisplayMode() {
    int32_t id = 0;
    uint32_t flags = 0;
    if (Utils::readInitialModeId(&id) != OK || id < 0) {
        if (disp_api_get_default_display_mode(mHandle, 0, &id, &flags) == OK && id >= 0) {
            return Utils::writeInitialModeId(id);
        } else {
            return Utils::writeInitialModeId(id = 0);
        }
    }
    return OK;
}

status_t SDMColor::getColorBalanceRange(Range& range) {
    status_t rc = disp_api_get_global_color_balance_range(mHandle, 0, &range);
    ALOGV("getColorBalanceRange: min=%d max=%d step=%d", range.min, range.max, range.step);
    return rc;
}

status_t SDMColor::getPictureAdjustment(HSIC& hsic) {
    uint32_t enable = 0;
    hsic_config config;
    memset(&config, 0, sizeof(struct hsic_config));

    status_t rc = disp_api_get_global_pa_config(mHandle, 0, &enable, &config);
    if (rc == OK) {
        hsic.hue = config.data.hue;
        hsic.saturation = config.data.saturation;
        hsic.intensity = config.data.intensity;
        hsic.contrast = config.data.contrast;
        hsic.saturationThreshold = config.data.saturationThreshold;
    }
    return rc;
}

//----------------------------------------------------------------------------/

Return<void> SDMColor::native_getDisplayModes(native_getDisplayModes_cb _hidl_cb) {
    hidl_vec<DisplayMode> profiles;
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        vector<sp<disp_mode>> spProfiles;
        rc = getDisplayModes(spProfiles);
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

Return<void> SDMColor::native_getCurrentDisplayMode(native_getCurrentDisplayMode_cb _hidl_cb) {
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        _hidl_cb(modePointerToObj(getCurrentDisplayMode()));
    }
    return Void();
}

Return<void> SDMColor::native_getDefaultDisplayMode(native_getDefaultDisplayMode_cb _hidl_cb) {
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        _hidl_cb(modePointerToObj(getDefaultDisplayMode()));
    }
    return Void();
}

Return<bool> SDMColor::native_setDisplayMode(int32_t modeID, bool makeDefault) {
    status_t rc = OK;
    Mutex::Autolock _l(mLock);

    if (!check(Feature::DISPLAY_MODES)) {
        return FAIL;
    }

    if (modeID == mActiveModeId) {
        return SUCCESS;
    }

    sp<disp_mode> mode = getDisplayModeById(modeID);
    if (mode == nullptr) {
        goto fail;
    }

    ALOGV("setDisplayMode: current mode=%d", mActiveModeId);

    if (mActiveModeId >= 0) {
        sp<disp_mode> oldMode = getCurrentDisplayMode();
        ALOGV("setDisplayMode: oldMode=%d flags=%d", oldMode->id, oldMode->privFlags);
        if (oldMode->privFlags == PRIV_MODE_FLAG_SYSFS ||
                mode->privFlags == PRIV_MODE_FLAG_SYSFS) {
            ALOGV("disabling old mode");
            rc = setModeState(oldMode, false);
            if (rc != OK) {
                ALOGE("Failed to disable previous mode! err=%d", rc);
                goto fail;
            }
        }
    }

    rc = setModeState(mode, true);
    if (rc == OK) {
        mActiveModeId = mode->id;
        if (makeDefault) {
            rc = Utils::writeLocalModeId(mode->id);
            if (rc != OK) {
                ALOGE("failed to save mode! %d", rc);
                goto fail;
            }
            if (mode->privFlags == PRIV_MODE_FLAG_SDM) {
                rc = disp_api_set_default_display_mode(mHandle, 0, mode->id, 0);
                if (rc != OK) {
                    ALOGE("failed to save mode! %d", rc);
                    goto fail;
                }
            }
        }
        HSIC tmp;
        rc = getPictureAdjustment(tmp);
        if (rc != OK) {
            ALOGE("failed to retrieve picture adjustment after mode setting!");
        } else {
            ALOGV("new default PA: %d %f %f %f %f", tmp.hue, tmp.saturation,
                    tmp.intensity, tmp.contrast, tmp.saturationThreshold);
            mDefaultPictureAdjustment = tmp;
        }
    } else {
        ALOGE("Failed to setModeState! err=%d", rc);
        goto fail;
    }

    ALOGV("setDisplayMode: %d default: %d flags: %d", modeID, makeDefault, mode->privFlags);
    return OK;

fail:
    error("Unable to set display mode!");
    return FAIL;
}

Return<bool> SDMColor::native_setAdaptiveBacklightEnabled(bool enabled) {
    status_t rc = FAIL;
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
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
    }
    return rc;
}

Return<bool> SDMColor::native_isAdaptiveBacklightEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
        return mCachedFOSSStatus;
    }
    return false;
}

Return<void> SDMColor::native_getColorBalanceRange(native_getColorBalanceRange_cb _hidl_cb) {
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_TEMPERATURE)) {
        Range range;
        status_t rc = getColorBalanceRange(range);
        if (rc != OK) {
            error("Unable to fetch color balance range!");
        } else {
            _hidl_cb(range);
        }
    }

    return Void();
}

Return<int32_t> SDMColor::native_getColorBalance() {
    int32_t value = -1;
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_TEMPERATURE)) {
        uint32_t flags = 0;
        if (disp_api_get_global_color_balance(mHandle, 0, &value, &flags) != OK) {
            value = 0;
        }
    }
    return value;
}

Return<bool> SDMColor::native_setColorBalance(int32_t value) {
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_TEMPERATURE)) {
        if (disp_api_set_global_color_balance(mHandle, 0, value, 0) != OK) {
            error("Unable to set color balance!");
        }
    }
    return FAIL;
}

Return<bool> SDMColor::native_setPictureAdjustment(const HSIC& hsic) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        hsic_config config;
        memset(&config, 0, sizeof(struct hsic_config));
        config.data.hue = hsic.hue;
        config.data.saturation = hsic.saturation;
        config.data.intensity = hsic.intensity;
        config.data.contrast = hsic.contrast;
        config.data.saturationThreshold = hsic.saturationThreshold;

        rc = disp_api_set_global_pa_config(mHandle, 0, 1, &config);
        if (rc != OK) {
            error("Unable to set picture adjustment!");
        }
    }
    return rc == OK;
}

Return<void> SDMColor::native_getPictureAdjustment(native_getPictureAdjustment_cb _hidl_cb) {
    HSIC hsic;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        status_t rc = getPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to get picture adjustment!");
        } else {
            _hidl_cb(hsic);
        }
    }
    return Void();
}

Return<void> SDMColor::native_getDefaultPictureAdjustment(native_getDefaultPictureAdjustment_cb _hidl_cb) {
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        _hidl_cb(mDefaultPictureAdjustment);
    }
    return Void();
}

Return<void> SDMColor::native_getHueRange(native_getHueRange_cb _hidl_cb) {
    HSICRanges ranges;
    if (getPictureAdjustmentRanges(ranges) == OK) {
        _hidl_cb(ranges.hue);
    }
    return Void();
}

Return<void> SDMColor::native_getSaturationRange(native_getSaturationRange_cb _hidl_cb) {
    HSICRanges ranges;
    if (getPictureAdjustmentRanges(ranges) == OK) {
        _hidl_cb(ranges.saturation);
    }
    return Void();
}

Return<void> SDMColor::native_getIntensityRange(native_getIntensityRange_cb _hidl_cb) {
    HSICRanges ranges;
    if (getPictureAdjustmentRanges(ranges) == OK) {
        _hidl_cb(ranges.intensity);
    }
    return Void();
}

Return<void> SDMColor::native_getContrastRange(native_getContrastRange_cb _hidl_cb) {
    HSICRanges ranges;
    if (getPictureAdjustmentRanges(ranges) == OK) {
        _hidl_cb(ranges.contrast);
    }
    return Void();
}

Return<void> SDMColor::native_getSaturationThresholdRange(native_getSaturationThresholdRange_cb _hidl_cb) {
    HSICRanges ranges;
    if (getPictureAdjustmentRanges(ranges) == OK) {
        _hidl_cb(ranges.saturationThreshold);
    }
    return Void();
}

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor
