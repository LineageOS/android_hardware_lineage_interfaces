/*
 * Copyright (C) 2018 The LineageOS Project
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

#define LOG_TAG "LiveDisplay-LegacyMM-Impl"

#include "controller/LegacyMMController.h"

#include <android-base/logging.h>

#include <dlfcn.h>

#define LOAD_SDM_FUNCTION(name) \
    mFn_##name = loadFunction<disp_api_##name>(mHandle, "disp_api_" #name);

#define CLOSE_SDM_FUNCTION(name) mFn_##name = nullptr;

#define FOR_EACH_FUNCTION(MACRO)    \
    MACRO(init)                     \
    MACRO(get_color_balance_range)  \
    MACRO(set_color_balance)        \
    MACRO(get_color_balance)        \
    MACRO(get_num_display_modes)    \
    MACRO(get_display_modes)        \
    MACRO(get_active_display_mode)  \
    MACRO(set_active_display_mode)  \
    MACRO(set_default_display_mode) \
    MACRO(get_default_display_mode) \
    MACRO(get_pa_range)             \
    MACRO(get_pa_config)            \
    MACRO(set_pa_config)            \
    MACRO(supported)

#define CONTROLLER_CHECK(function, ...)    \
    if (mFn_##function == nullptr) {       \
        return -1;                         \
    }                                      \
    int err = mFn_##function(__VA_ARGS__); \
    if (err != 0) {                        \
        return err;                        \
    }                                      \
    return 0;

namespace {
constexpr char kFilename[] = "libmm-disp-apis.so";
template <typename Function>
Function loadFunction(std::shared_ptr<void> handle, const char* name) {
    void* fn = dlsym(handle.get(), name);
    if (fn == nullptr) {
        LOG(ERROR) << "loadFunction -- failed to load function " << name;
    }
    return reinterpret_cast<Function>(fn);
}
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

LegacyMMController::LegacyMMController() {
    mHandle = openlib();
    if (mHandle != nullptr) {
        FOR_EACH_FUNCTION(LOAD_SDM_FUNCTION)
    }
}

std::shared_ptr<void> LegacyMMController::openlib() {
    std::shared_ptr<void> handle(dlopen(kFilename, RTLD_NOW), [this](void* p) {
        FOR_EACH_FUNCTION(CLOSE_SDM_FUNCTION)
        if (p != nullptr) {
            int err = dlclose(p);
            p = nullptr;
            if (err != 0) {
                LOG(ERROR) << "DLCLOSE failed for " << kFilename;
            }
        }
    });
    if (handle == nullptr) {
        LOG(ERROR) << "DLOPEN failed for " << kFilename << " (" << dlerror() << ")";
        return nullptr;
    }
    return handle;
}

LegacyMMController& LegacyMMController::getInstance() {
    static LegacyMMController instance{};
    return instance;
}

int32_t LegacyMMController::init(int32_t initialize) {
    CONTROLLER_CHECK(init, initialize);
}

int32_t LegacyMMController::get_color_balance_range(int32_t disp_id, void* range) {
    CONTROLLER_CHECK(get_color_balance_range, disp_id, range);
}

int32_t LegacyMMController::set_color_balance(int32_t disp_id, int32_t warmness) {
    CONTROLLER_CHECK(set_color_balance, disp_id, warmness);
}

int32_t LegacyMMController::get_color_balance(int32_t disp_id, int32_t* warmness) {
    CONTROLLER_CHECK(get_color_balance, disp_id, warmness);
}

int32_t LegacyMMController::get_num_display_modes(int32_t disp_id, int32_t mode_type,
                                                  uint32_t* mode_cnt) {
    CONTROLLER_CHECK(get_num_display_modes, disp_id, mode_type, mode_cnt);
}

int32_t LegacyMMController::get_display_modes(int32_t disp_id, int32_t mode_type, void* modes,
                                              int32_t mode_cnt) {
    CONTROLLER_CHECK(get_display_modes, disp_id, mode_type, modes, mode_cnt);
}

int32_t LegacyMMController::get_active_display_mode(int32_t disp_id, int32_t* mode_id,
                                                    uint32_t* mask) {
    CONTROLLER_CHECK(get_active_display_mode, disp_id, mode_id, mask);
}

int32_t LegacyMMController::set_active_display_mode(int32_t disp_id, int32_t mode_id) {
    CONTROLLER_CHECK(set_active_display_mode, disp_id, mode_id);
}

int32_t LegacyMMController::set_default_display_mode(int32_t disp_id, int32_t mode_id) {
    CONTROLLER_CHECK(set_default_display_mode, disp_id, mode_id);
}

int32_t LegacyMMController::get_default_display_mode(int32_t disp_id, int32_t* mode_id) {
    CONTROLLER_CHECK(get_default_display_mode, disp_id, mode_id);
}

int32_t LegacyMMController::get_pa_range(int32_t disp_id, void* range) {
    CONTROLLER_CHECK(get_pa_range, disp_id, range);
}

int32_t LegacyMMController::get_pa_config(int32_t disp_id, void* cfg) {
    CONTROLLER_CHECK(get_pa_config, disp_id, cfg);
}

int32_t LegacyMMController::set_pa_config(int32_t disp_id, void* cfg) {
    CONTROLLER_CHECK(set_pa_config, disp_id, cfg);
}

int32_t LegacyMMController::supported(int32_t disp_id, uint32_t feature_id) {
    CONTROLLER_CHECK(supported, disp_id, feature_id);
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
