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

#define LOG_TAG "LiveDisplay-HIDL"

#include <dlfcn.h>
#include <log/log.h>

#include "LiveDisplay.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::OK;
using ::android::NO_INIT;

status_t LiveDisplay::loadVendorLibrary() {
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
    disp_api_set_global_pa_config = (int32_t(*)(int64_t, uint32_t, uint32_t, const void*))dlsym(
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

Return<void> LiveDisplay::init(uint32_t flags, init_cb _hidl_cb)
{
    int64_t hctx = 0;
    status_t rc = loadVendorLibrary();

    if (rc != OK) {
        goto fail;
    }

    rc = disp_api_init(&hctx, flags);
    if (rc != OK) {
        goto fail;
    }

fail:
    _hidl_cb(rc, hctx);
    return Void();
}

Return<int32_t> LiveDisplay::deinit(int64_t hctx, uint32_t flags)
{
    if (disp_api_deinit == NULL) {
        return NO_INIT;
    }
    return disp_api_deinit(hctx, flags);
}

Return<void> LiveDisplay::getGlobalColorBalanceRange(int64_t hctx, uint32_t disp_id,
                                                     getGlobalColorBalanceRange_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    disp_range range;

    memset(&range, 0, sizeof(range));
    if (disp_api_get_global_color_balance_range == NULL) {
        goto fail;
    }

    rc = disp_api_get_global_color_balance_range(hctx, disp_id, &range);

fail:
    _hidl_cb(rc, range);
    return Void();
}

Return<int32_t> LiveDisplay::setGlobalColorBalance(int64_t hctx, uint32_t disp_id, int32_t warmness, uint32_t flags)
{
    if (disp_api_set_global_color_balance == NULL) {
        return NO_INIT;
    }
    return disp_api_set_global_color_balance(hctx, disp_id, warmness, flags);
}

Return<void> LiveDisplay::getGlobalColorBalance(int64_t hctx, uint32_t disp_id,
                                                getGlobalColorBalance_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    int32_t warmness = -1;
    uint32_t flags = 0;

    if (disp_api_get_global_color_balance == NULL) {
        warmness = 0;
        goto fail;
    }

    rc = disp_api_get_global_color_balance(hctx, disp_id, &warmness, &flags);

fail:
    _hidl_cb(rc, warmness, flags);
    return Void();
}

Return<void> LiveDisplay::getNumDisplayModes(int64_t hctx, uint32_t disp_id, int32_t mode_type,
                                             getNumDisplayModes_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    int32_t mode_cnt = 0;
    uint32_t flags = 0;

    if (disp_api_get_num_display_modes == NULL) {
        goto fail;
    }

    rc = disp_api_get_num_display_modes(hctx, disp_id, mode_type, &mode_cnt, &flags);
    
fail:
    _hidl_cb(rc, mode_cnt, flags);
    return Void();
}

Return<void> LiveDisplay::getDisplayModes(int64_t hctx, uint32_t disp_id, int32_t mode_type, int32_t mode_cnt,
                                          getDisplayModes_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    hidl_vec<disp_mode> modes;
    uint32_t flags = 0;

    if (disp_api_get_display_modes == NULL) {
        _hidl_cb(rc, modes, flags);
        return Void();
    }

    struct disp_mode_tmp {
        int32_t id;
        int32_t type;
        uint32_t name_len;
        char *name;
    };

    disp_mode_tmp* tmp = new disp_mode_tmp[mode_cnt];
    memset(tmp, 0, sizeof(disp_mode_tmp) * mode_cnt);
    for (int i = 0; i < mode_cnt; i++) {
        tmp[i].id = -1;
        tmp[i].name = new char[128];
        tmp[i].name_len = 128;
    }

    rc = disp_api_get_display_modes(hctx, disp_id, mode_type, tmp, mode_cnt, &flags);
    if (rc == 0) {
        modes.resize(mode_cnt);
        for (int i = 0; i < mode_cnt; i++) {
            modes[i].id = tmp[i].id;
            modes[i].type = tmp[i].type;
            modes[i].name_len = tmp[i].name_len;
            modes[i].name = tmp[i].name;
        }
    }

    for (int i = 0; i < mode_cnt; i++) {
        delete tmp[i].name;
    }
    delete[] tmp;

    _hidl_cb(rc, modes, flags);
    return Void();
}

Return<void> LiveDisplay::getActiveDisplayMode(int64_t hctx, uint32_t disp_id,
                                               getActiveDisplayMode_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    int32_t mode_id = 0;
    uint32_t mask = 0;
    uint32_t flags = 0;

    if (disp_api_get_active_display_mode == NULL) {
        goto fail;
    }

    rc = disp_api_get_active_display_mode(hctx, disp_id, &mode_id, &mask, &flags);

fail:
    _hidl_cb(rc, mode_id, mask, flags);
    return Void();
}

Return<int32_t> LiveDisplay::setActiveDisplayMode(int64_t hctx, uint32_t disp_id, int32_t mode_id, uint32_t flags)
{
    if (disp_api_set_active_display_mode == NULL) {
        return NO_INIT;
    }
    return disp_api_set_active_display_mode(hctx, disp_id, mode_id, flags);
}

Return<void> LiveDisplay::getDefaultDisplayMode(int64_t hctx, uint32_t disp_id,
                                                getDefaultDisplayMode_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    int32_t mode_id = 0;
    uint32_t flags = 0;

    if (disp_api_get_default_display_mode == NULL) {
        goto fail;
    }

    rc = disp_api_get_default_display_mode(hctx, disp_id, &mode_id, &flags);

fail:
    _hidl_cb(rc, mode_id, flags);
    return Void();
}

Return<int32_t> LiveDisplay::setDefaultDisplayMode(int64_t hctx, uint32_t disp_id, int32_t mode_id, uint32_t flags)
{
    if (disp_api_set_default_display_mode == NULL) {
        return NO_INIT;
    }
    return disp_api_set_default_display_mode(hctx, disp_id, mode_id, flags);
}

Return<void> LiveDisplay::getGlobalPARange(int64_t hctx, uint32_t disp_id,
                                           getGlobalPARange_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    disp_pa_range range;

    memset(&range, 0, sizeof(range));
    if (disp_api_get_global_pa_range == NULL) {
        goto fail;
    }

    rc = disp_api_get_global_pa_range(hctx, disp_id, &range);

fail:
    _hidl_cb(rc, range);
    return Void();
}

Return<void> LiveDisplay::getGlobalPAConfig(int64_t hctx, uint32_t disp_id,
                                            getGlobalPAConfig_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    uint32_t enable = 0;
    disp_pa_config cfg;

    memset(&cfg, 0, sizeof(cfg));
    if (disp_api_get_global_pa_config == NULL) {
        goto fail;
    }

    rc = disp_api_get_global_pa_config(hctx, disp_id, &enable, &cfg);

fail:
    _hidl_cb(rc, enable, cfg);
    return Void();
}

Return<int32_t> LiveDisplay::setGlobalPAConfig(int64_t hctx, uint32_t disp_id,
                                                uint32_t enable, const disp_pa_config& cfg)
{
    if (disp_api_set_global_pa_config == NULL) {
        return NO_INIT;
    }
    return disp_api_set_global_pa_config(hctx, disp_id, enable, &cfg);
}

Return<void> LiveDisplay::getFeatureVersion(int64_t hctx, uint32_t feature_id,
                                            getFeatureVersion_cb _hidl_cb)
{
    status_t rc = NO_INIT;
    version_info ver;
    uint32_t flags = 0;

    memset(&ver, 0, sizeof(ver));
    if (disp_api_get_feature_version == NULL) {
        goto fail;
    }

    rc = disp_api_get_feature_version(hctx, feature_id, &ver, &flags);

fail:
    _hidl_cb(rc, ver, flags);
    return Void();
}

LiveDisplay::~LiveDisplay() {
    if (mLibHandle != NULL) {
        dlclose(mLibHandle);
    }
}

ILiveDisplay *HIDL_FETCH_ILiveDisplay(const char* /* name */)
{
    return new LiveDisplay();
}

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor
