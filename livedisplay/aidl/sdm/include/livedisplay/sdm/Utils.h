/*
 * SPDX-FileCopyrightText: 2016 The CyanogenMod Project
 * SPDX-FileCopyrightText: 2017-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "SDMController.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {
namespace utils {

using ::android::status_t;

enum FeatureVerSw {
    FEATURE_VER_SW_API = 0,
    FEATURE_VER_SW_PA_API,
    FEATURE_VER_SW_MEMCOLOR_API,
    FEATURE_VER_SW_COLORBAL_API,
    FEATURE_VER_SW_SAVEMODES_API,
    FEATURE_VER_SW_ADAPT_BACKLIGHT_API,
    FEATURE_VER_SW_SVI_API,
    FEATURE_VER_SW_DISP_INFO_API
};

status_t CheckFeatureVersion(const std::shared_ptr<SDMController>& controller,
                             FeatureVerSw feature);

}  // namespace utils
}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
