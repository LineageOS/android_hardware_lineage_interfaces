/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-impl-sdm"

#include "livedisplay/sdm/DisplayModes.h"
#include "livedisplay/sdm/Utils.h"

#include <android-base/logging.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {

using ::android::OK;

DisplayModes::DisplayModes(std::shared_ptr<SDMController> controller)
    : controller_(std::move(controller)) {
    if (!isReady()) {
        LOG(FATAL) << "DisplayModes backend not ready, exiting.";
    }

    DisplayMode mode = getDefaultDisplayModeInternal();
    if (mode.id >= 0) {
        bool aidl_return;
        setDisplayMode(mode.id, false, &aidl_return);
    }
}

bool DisplayModes::isReady() {
    static int supported = -1;

    if (supported >= 0) {
        return supported;
    }

    if (utils::CheckFeatureVersion(controller_, utils::FEATURE_VER_SW_SAVEMODES_API) != OK) {
        supported = 0;
        return false;
    }

    int32_t count = 0;
    if (controller_->getNumDisplayModes(&count) != OK) {
        count = 0;
    }
    supported = (count > 0);

    return supported;
}

std::vector<DisplayMode> DisplayModes::getDisplayModesInternal() {
    std::vector<DisplayMode> modes;
    int32_t count = 0;

    if (controller_->getNumDisplayModes(&count) != OK || count == 0) {
        return modes;
    }

    std::vector<SdmDispMode> tmp_modes(count);
    if (controller_->getDisplayModes(tmp_modes.data(), count) == OK) {
        for (auto&& mode : tmp_modes) {
            modes.push_back({mode.id, mode.name});
        }
    }

    return modes;
}

DisplayMode DisplayModes::getDisplayModeById(int32_t id) {
    std::vector<DisplayMode> modes = getDisplayModesInternal();

    for (auto&& mode : modes) {
        if (mode.id == id) {
            return mode;
        }
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModes::getCurrentDisplayModeInternal() {
    int32_t id = 0;

    if (controller_->getActiveDisplayMode(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModes::getDefaultDisplayModeInternal() {
    int32_t id = 0;

    if (controller_->getDefaultDisplayMode(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return DisplayMode{-1, ""};
}

// Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayModes follow.
ndk::ScopedAStatus DisplayModes::getDisplayModes(std::vector<DisplayMode>* _aidl_return) {
    *_aidl_return = getDisplayModesInternal();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getCurrentDisplayMode(DisplayMode* _aidl_return) {
    *_aidl_return = getCurrentDisplayModeInternal();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getDefaultDisplayMode(DisplayMode* _aidl_return) {
    *_aidl_return = getDefaultDisplayModeInternal();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault,
                                                bool* _aidl_return) {
    DisplayMode current_mode = getCurrentDisplayModeInternal();

    if (current_mode.id >= 0 && current_mode.id == modeID) {
        *_aidl_return = true;
        return ndk::ScopedAStatus::ok();
    }

    DisplayMode mode = getDisplayModeById(modeID);
    if (mode.id < 0) {
        *_aidl_return = false;
        return ndk::ScopedAStatus::ok();
    }

    if (controller_->setActiveDisplayMode(modeID) != OK) {
        *_aidl_return = false;
        return ndk::ScopedAStatus::ok();
    }

    if (makeDefault && controller_->setDefaultDisplayMode(modeID) != OK) {
        *_aidl_return = false;
        return ndk::ScopedAStatus::ok();
    }

    if (on_display_mode_set_) {
        on_display_mode_set_();
    }

    *_aidl_return = true;

    return ndk::ScopedAStatus::ok();
}

void DisplayModes::registerDisplayModeSetCallback(DisplayModeSetCallback callback) {
    on_display_mode_set_ = callback;
}

}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
