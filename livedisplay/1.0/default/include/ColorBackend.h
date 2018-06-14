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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLORBACKEND_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLORBACKEND_H

#include "Types.h"

#include <vector>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

class ColorBackend {
  public:
    virtual android::status_t setAdaptiveBacklightEnabled(bool enabled) = 0;
    virtual bool isAdaptiveBacklightEnabled() = 0;

    virtual android::status_t setOutdoorModeEnabled(bool enabled) = 0;
    virtual bool isOutdoorModeEnabled() = 0;

    virtual android::status_t getColorBalanceRange(Range& range) = 0;
    virtual android::status_t setColorBalance(int32_t balance) = 0;
    virtual int32_t getColorBalance() = 0;

    virtual android::status_t getDisplayModes(std::vector<android::sp<disp_mode>>& profiles) = 0;
    virtual android::status_t setDisplayMode(int32_t modeID, bool makeDefault) = 0;
    virtual android::sp<disp_mode> getCurrentDisplayMode() = 0;
    virtual android::sp<disp_mode> getDefaultDisplayMode() = 0;

    virtual android::status_t getPictureAdjustmentRanges(HSICRanges& ranges) = 0;
    virtual android::status_t getPictureAdjustment(HSIC& hsic) = 0;
    virtual HSIC getDefaultPictureAdjustment() = 0;
    virtual android::status_t setPictureAdjustment(const HSIC& hsic) = 0;

    virtual android::status_t initialize() = 0;
    virtual android::status_t deinitialize() = 0;
    virtual bool hasFeature(Feature feature) = 0;

    virtual ~ColorBackend() {
    }
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLORBACKEND_H
