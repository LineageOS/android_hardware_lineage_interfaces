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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDM_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDM_H

#include "LiveDisplayBackend.h"

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

using ::android::NO_INIT;

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

class SDM : public LiveDisplayBackend
{
public:
    virtual status_t getDisplayModes(vector<sp<disp_mode>>& profiles) override;
    virtual sp<disp_mode> getCurrentDisplayMode() override;
    virtual sp<disp_mode> getDefaultDisplayMode() override;
    virtual status_t setDisplayMode(int32_t modeID, bool makeDefault) override;
    
    virtual status_t setAdaptiveBacklightEnabled(bool enabled) override;
    virtual bool isAdaptiveBacklightEnabled() override;

    virtual status_t setOutdoorModeEnabled(bool /* enabled */) override {
        return NO_INIT;
    }
    virtual bool isOutdoorModeEnabled() override {
        return false;
    }
    
    virtual status_t getColorBalanceRange(Range& range) override;
    virtual int32_t getColorBalance() override;
    virtual status_t setColorBalance(int32_t balance) override;
    
    virtual status_t getPictureAdjustmentRanges(HSICRanges& ranges) override;
    virtual status_t setPictureAdjustment(const HSIC& hsic) override;
    virtual status_t getPictureAdjustment(HSIC& hsic) override;
    virtual HSIC getDefaultPictureAdjustment() override;

    virtual bool hasFeature(Feature f);
    virtual status_t initialize();
    virtual status_t deinitialize();

    ~SDM();

private:
    status_t loadVendorLibrary();

    status_t saveInitialDisplayMode();
    uint32_t getNumDisplayModes();
    uint32_t getNumSDMDisplayModes();

    sp<disp_mode> getDisplayModeById(int32_t id);
    status_t setModeState(sp<disp_mode> mode, bool state);
    sp<disp_mode> getLocalSRGBMode();
    sp<disp_mode> getLocalDCIP3Mode();

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

#endif // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDM_H
