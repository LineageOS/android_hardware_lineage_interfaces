/*
 * Copyright (C) 2017 The LineageOS Project
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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_LIVEDISPLAY_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_LIVEDISPLAY_H

#include <vendor/lineage/livedisplay/1.0/IColor.h>

#define SDM_DISP_LIB "libsdm-disp-vndapis.so"
#define SDM_DISP_LEGACY_LIB "libsdm-disp-apis.so"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::status_t;
using ::vendor::lineage::livedisplay::V1_0::IColor;

struct SDMColor : public IColor
{
    Return<void> init(uint32_t flags, init_cb _hidl_cb) override;
    Return<int32_t> deinit(int64_t hctx, uint32_t flags) override;
    Return<void> getGlobalColorBalanceRange(int64_t hctx, uint32_t disp_id,
                                            getGlobalColorBalanceRange_cb _hidl_cb) override;
    Return<int32_t> setGlobalColorBalance(int64_t hctx, uint32_t disp_id, int32_t warmness, uint32_t flags) override;
    Return<void> getGlobalColorBalance(int64_t hctx, uint32_t disp_id,
                                       getGlobalColorBalance_cb _hidl_cb) override;
    Return<void> getNumDisplayModes(int64_t hctx, uint32_t disp_id, int32_t mode_type,
                                    getNumDisplayModes_cb _hidl_cb) override;
    Return<void> getDisplayModes(int64_t hctx, uint32_t disp_id, int32_t mode_type, int32_t mode_cnt,
                                 getDisplayModes_cb _hidl_cb) override;
    Return<void> getActiveDisplayMode(int64_t hctx, uint32_t disp_id,
                                      getActiveDisplayMode_cb _hidl_cb) override;
    Return<int32_t> setActiveDisplayMode(int64_t hctx, uint32_t disp_id, int32_t mode_id, uint32_t flags) override;
    Return<void> getDefaultDisplayMode(int64_t hctx, uint32_t disp_id,
                                       getDefaultDisplayMode_cb _hidl_cb) override;
    Return<int32_t> setDefaultDisplayMode(int64_t hctx, uint32_t disp_id, int32_t mode_id, uint32_t flags) override;
    Return<void> getGlobalPARange(int64_t hctx, uint32_t disp_id,
                                  getGlobalPARange_cb _hidl_cb) override;
    Return<void> getGlobalPAConfig(int64_t hctx, uint32_t disp_id,
                                   getGlobalPAConfig_cb _hidl_cb) override;
    Return<int32_t> setGlobalPAConfig(int64_t hctx, uint32_t disp_id, uint32_t enable, const disp_pa_config& cfg) override;
    Return<void> getFeatureVersion(int64_t hctx, uint32_t feature_id,
                                   getFeatureVersion_cb _hidl_cb) override;

    SDMColor();
    ~SDMColor();
private:
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
    int32_t (*disp_api_set_global_pa_config)(int64_t, uint32_t, uint32_t, const void*);
    int32_t (*disp_api_get_feature_version)(int64_t, uint32_t, void*, uint32_t*);

    void loadVendorLibrary();
};

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor

#endif // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_LIVEDISPLAY_H
