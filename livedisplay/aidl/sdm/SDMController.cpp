/*
 * SPDX-FileCopyrightText: 2018-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-impl-sdm"

#include "livedisplay/sdm/SDMController.h"

#include <android-base/logging.h>
#include <dlfcn.h>

#include <memory>

#define LOAD_SDM_FUNCTION(name) \
    fn_##name##_ = LoadFunction<disp_api_##name>(handle_.get(), "disp_api_" #name);

#define CLOSE_SDM_FUNCTION(name) fn_##name##_ = nullptr;

#define FOR_EACH_FUNCTION(MACRO)    \
    MACRO(init)                     \
    MACRO(deinit)                   \
    MACRO(get_num_display_modes)    \
    MACRO(get_display_modes)        \
    MACRO(get_active_display_mode)  \
    MACRO(set_active_display_mode)  \
    MACRO(set_default_display_mode) \
    MACRO(get_default_display_mode) \
    MACRO(get_global_pa_range)      \
    MACRO(get_global_pa_config)     \
    MACRO(set_global_pa_config)     \
    MACRO(get_feature_version)

#define CONTROLLER_CHECK(function, ...)      \
    if (fn_##function##_ == nullptr) {       \
        return ::android::NO_INIT;           \
    }                                        \
    int err = fn_##function##_(__VA_ARGS__); \
    if (err != 0) {                          \
        return ::android::BAD_VALUE;         \
    }                                        \
    return android::OK;

namespace {
constexpr const int kDisplayId = 0;

void* LoadDispApisLib() {
    auto handle = dlopen("libsdm-disp-vndapis.so", RTLD_NOW);
    if (handle != nullptr) {
        LOG(DEBUG) << "Loaded: libsdm-disp-vndapis.so";
    } else {
        LOG(INFO) << "Can not load libsdm-disp-vndapis.so (" << dlerror() << ")";
    }
    return handle;
}

template <typename Function>
Function LoadFunction(void* handle, const char* name) {
    void* fn = dlsym(handle, name);
    if (fn == nullptr) {
        LOG(ERROR) << "LoadFunction -- failed to load function " << name;
    }
    return reinterpret_cast<Function>(fn);
}
}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {

SDMController::SDMController()
    : handle_(LoadDispApisLib(), [this](void* p) {
          FOR_EACH_FUNCTION(CLOSE_SDM_FUNCTION)
          if (p != nullptr) {
              int err = dlclose(p);
              p = nullptr;
              if (err != 0) {
                  LOG(ERROR) << "Failed to close disp api lib.";
              }
          }
      }) {
    if (handle_ == nullptr) {
        // Terminate the program on failure
        LOG(FATAL) << "Failed to load SDM display lib, exiting. (" << dlerror() << ")";
    }

    FOR_EACH_FUNCTION(LOAD_SDM_FUNCTION)

    // Initialize SDM backend
    if (init() != android::OK) {
        // Terminate the program on failure
        LOG(FATAL) << "Failed to initialize SDM backend";
    }
}

SDMController::~SDMController() {
    deinit();
}

status_t SDMController::init() {
    CONTROLLER_CHECK(init, &hctx_, 0);
}

status_t SDMController::deinit() {
    CONTROLLER_CHECK(deinit, hctx_, 0);
}

status_t SDMController::getNumDisplayModes(int32_t* mode_cnt) {
    uint32_t flags = 0;
    CONTROLLER_CHECK(get_num_display_modes, hctx_, kDisplayId, 0, mode_cnt, &flags);
}

status_t SDMController::getDisplayModes(SdmDispMode* modes, int32_t mode_cnt) {
    uint32_t flags = 0;
    CONTROLLER_CHECK(get_display_modes, hctx_, kDisplayId, 0, modes, mode_cnt, &flags);
}

status_t SDMController::getActiveDisplayMode(int32_t* mode_id) {
    uint32_t mask = 0, flags = 0;
    CONTROLLER_CHECK(get_active_display_mode, hctx_, kDisplayId, mode_id, &mask, &flags);
}

status_t SDMController::setActiveDisplayMode(int32_t mode_id) {
    CONTROLLER_CHECK(set_active_display_mode, hctx_, kDisplayId, mode_id, 0);
}

status_t SDMController::setDefaultDisplayMode(int32_t mode_id) {
    CONTROLLER_CHECK(set_default_display_mode, hctx_, kDisplayId, mode_id, 0);
}

status_t SDMController::getDefaultDisplayMode(int32_t* mode_id) {
    uint32_t flags = 0;
    CONTROLLER_CHECK(get_default_display_mode, hctx_, kDisplayId, mode_id, &flags);
}

status_t SDMController::getGlobalPaRange(HsicRanges* range) {
    CONTROLLER_CHECK(get_global_pa_range, hctx_, kDisplayId, range);
}

status_t SDMController::getGlobalPaConfig(HsicConfig* cfg) {
    uint32_t enable = 0;
    CONTROLLER_CHECK(get_global_pa_config, hctx_, kDisplayId, &enable, cfg);
}

status_t SDMController::setGlobalPaConfig(HsicConfig* cfg) {
    CONTROLLER_CHECK(set_global_pa_config, hctx_, kDisplayId, 1 /* enable */, cfg);
}

status_t SDMController::getFeatureVersion(uint32_t feature_id, SdmFeatureVersion* ver) {
    uint32_t flags = 0;
    CONTROLLER_CHECK(get_feature_version, hctx_, feature_id, ver, &flags);
}

}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
