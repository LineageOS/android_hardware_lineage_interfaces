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

DisplayModes::DisplayModes(std::shared_ptr<SDMController> controller) : controller_(controller) {
    if (!isReady()) {
        LOG(FATAL) << "DisplayModes backend not ready, exiting.";
    }

    if (auto mode = getDefaultDisplayModeInternal()) {
        setDisplayMode(mode->id, false);
    }
}

bool DisplayModes::isReady() {
    if (utils::CheckFeatureVersion(controller_, utils::FEATURE_VER_SW_SAVEMODES_API) != OK) {
        return false;
    }

    int32_t count = 0;
    if (controller_->getNumDisplayModes(&count) != OK) {
        return false;
    }

    return count > 0;
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

std::optional<DisplayMode> DisplayModes::getDisplayModeById(int32_t id) {
    std::vector<DisplayMode> modes = getDisplayModesInternal();

    for (auto&& mode : modes) {
        if (mode.id == id) {
            return mode;
        }
    }

    return {};
}

std::optional<DisplayMode> DisplayModes::getCurrentDisplayModeInternal() {
    int32_t id = 0;
    if (controller_->getActiveDisplayMode(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return {};
}

std::optional<DisplayMode> DisplayModes::getDefaultDisplayModeInternal() {
    int32_t id = 0;
    if (controller_->getDefaultDisplayMode(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return {};
}

// Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayModes follow.
ndk::ScopedAStatus DisplayModes::getDisplayModes(std::vector<DisplayMode>* _aidl_return) {
    *_aidl_return = getDisplayModesInternal();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getCurrentDisplayMode(DisplayMode* _aidl_return) {
    if (auto modes = getCurrentDisplayModeInternal()) {
        *_aidl_return = *modes;
        return ndk::ScopedAStatus::ok();
    }

    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus DisplayModes::getDefaultDisplayMode(DisplayMode* _aidl_return) {
    if (auto modes = getDefaultDisplayModeInternal()) {
        *_aidl_return = *modes;
        return ndk::ScopedAStatus::ok();
    }

    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    auto current_mode = getCurrentDisplayModeInternal();

    if (current_mode && current_mode->id == modeID) {
        return ndk::ScopedAStatus::ok();
    }

    auto mode = getDisplayModeById(modeID);
    if (!mode) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (controller_->setActiveDisplayMode(modeID) != OK) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (makeDefault && controller_->setDefaultDisplayMode(modeID) != OK) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (on_display_mode_set_) {
        on_display_mode_set_();
    }

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
