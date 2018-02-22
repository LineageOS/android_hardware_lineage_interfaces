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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDMCOLOR_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDMCOLOR_H

#include <string>
#include <vector>
#include "Color.h"

#define SDM_DISP_LIB "libsdm-disp-vndapis.so"
#define SDM_DISP_LEGACY_LIB "libsdm-disp-apis.so"

#define DPPS_BUF_SIZE 64

#define FOSS_SUPPORTED "foss:support"
#define FOSS_ON "foss:on"
#define FOSS_OFF "foss:off"
#define FOSS_STATUS "foss:status"

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

using ::android::hardware::Return;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::vendor::lineage::livedisplay::V1_0::IColor;

using ::android::BAD_VALUE;
using ::android::NO_INIT;
using ::android::OK;
using ::android::sp;

using std::string;
using std::vector;

inline bool isNonZero(Range r) {
    return r.min != 0 || r.max != 0;
}

inline bool isNonZero(FloatRange r) {
    return r.min != 0.f || r.max != 0.f;
}

struct HSICRanges {
    HSICRanges() {
    }
    HSICRanges(Range _hue, FloatRange _saturation, FloatRange _intensity,
            FloatRange _contrast, FloatRange _saturationThreshold)
        : hue(_hue),
          saturation(_saturation),
          intensity(_intensity),
          contrast(_contrast),
          saturationThreshold(_saturationThreshold) {
    }

    Range hue;
    FloatRange saturation;
    FloatRange intensity;
    FloatRange contrast;
    FloatRange saturationThreshold;

    bool isValid() {
        return isNonZero(hue) && isNonZero(saturation) &&
               isNonZero(intensity) && isNonZero(contrast);
    }
};

struct hsic_int_range {
    int32_t max;
    int32_t min;
    uint32_t step;
};

struct hsic_float_range {
    float max;
    float min;
    float step;
};

struct hsic_ranges {
    uint32_t flags;
    struct hsic_int_range hue;
    struct hsic_float_range saturation;
    struct hsic_float_range intensity;
    struct hsic_float_range contrast;
    struct hsic_float_range saturationThreshold;
};

struct hsic_config {
    uint32_t flags;
    HSIC data;
};

struct disp_mode : public android::RefBase {
    int32_t id;
    string name;

    uint32_t privFlags;
    string privData;

    disp_mode() : id(-1) {}
};

class SDMColor : public Color
{
public:
    Return<void> native_getDisplayModes(native_getDisplayModes_cb _hidl_cb) override;
    Return<void> native_getCurrentDisplayMode(native_getCurrentDisplayMode_cb _hidl_cb) override;
    Return<void> native_getDefaultDisplayMode(native_getDefaultDisplayMode_cb _hidl_cb) override;
    Return<bool> native_setDisplayMode(int32_t modeID, bool makeDefault) override;
    
    Return<bool> native_setAdaptiveBacklightEnabled(bool enabled) override;
    Return<bool> native_isAdaptiveBacklightEnabled() override;

    Return<bool> native_setOutdoorModeEnabled(bool /* enabled */) override {
        return NO_INIT;
    }
    Return<bool> native_isOutdoorModeEnabled() override {
        return false;
    }
    
    Return<void> native_getColorBalanceRange(native_getColorBalanceRange_cb _hidl_cb) override;
    Return<int32_t> native_getColorBalance() override;
    Return<bool> native_setColorBalance(int32_t value) override;
    
    Return<bool> native_setPictureAdjustment(const HSIC& hsic) override;
    Return<void> native_getPictureAdjustment(native_getPictureAdjustment_cb _hidl_cb) override;
    Return<void> native_getDefaultPictureAdjustment(native_getDefaultPictureAdjustment_cb _hidl_cb) override;
    
    Return<void> native_getHueRange(native_getHueRange_cb _hidl_cb) override;
    Return<void> native_getSaturationRange(native_getSaturationRange_cb _hidl_cb) override;
    Return<void> native_getIntensityRange(native_getIntensityRange_cb _hidl_cb) override;
    Return<void> native_getContrastRange(native_getContrastRange_cb _hidl_cb) override;
    Return<void> native_getSaturationThresholdRange(native_getSaturationThresholdRange_cb _hidl_cb) override;

    ~SDMColor();

private:
    status_t loadVendorLibrary();

    virtual bool backendHasFeature(Feature f);
    virtual status_t initialize();
    virtual status_t deinitialize();

    status_t saveInitialDisplayMode();
    uint32_t getNumDisplayModes();
    uint32_t getNumSDMDisplayModes();

    sp<disp_mode> getDisplayModeById(int32_t id);
    status_t setModeState(sp<disp_mode> mode, bool state);
    status_t getDisplayModes(vector<sp<disp_mode>>& profiles);
    sp<disp_mode> getCurrentDisplayMode();
    sp<disp_mode> getDefaultDisplayMode();
    DisplayMode modePointerToObj(sp<disp_mode> mode);
    sp<disp_mode> getLocalSRGBMode();
    sp<disp_mode> getLocalDCIP3Mode();
    status_t getPictureAdjustmentRanges(HSICRanges& ranges);
    status_t getColorBalanceRange(Range& range);
    status_t getPictureAdjustment(HSIC& hsic);

    DisplayMode invalidDisplayMode();

    int64_t mHandle;
    bool mCachedFOSSStatus;
    int32_t mActiveModeId;

    HSIC mDefaultPictureAdjustment;

    void* mLibHandle;

    int32_t (*disp_api_init)(int64_t*, uint32_t);
    int32_t (*disp_api_deinit)(int64_t, uint32_t);
    int32_t (*disp_api_get_global_color_balance_range)(int64_t, uint32_t, void*);
    int32_t (*disp_api_set_global_color_balance)(int64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_get_global_color_balance)(int64_t, uint32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_get_num_display_modes)(int64_t, uint32_t, int32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_get_display_modes)(int64_t, uint32_t, int32_t, void*, int32_t, uint32_t*);
    int32_t (*disp_api_get_active_display_mode)(int64_t, uint32_t, int32_t*, uint32_t*, uint32_t*);
    int32_t (*disp_api_set_active_display_mode)(int64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_set_default_display_mode)(int64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_get_default_display_mode)(int64_t, uint32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_get_global_pa_range)(int64_t, uint32_t, void*);
    int32_t (*disp_api_get_global_pa_config)(int64_t, uint32_t, uint32_t*, void*);
    int32_t (*disp_api_set_global_pa_config)(int64_t, uint32_t, uint32_t, void*);
    int32_t (*disp_api_get_feature_version)(int64_t, uint32_t, void*, uint32_t*);
};

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor

#endif // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDMCOLOR_H
