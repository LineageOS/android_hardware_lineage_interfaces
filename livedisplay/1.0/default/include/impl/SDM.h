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

#include "ColorBackend.h"
#include "controller/SDMController.h"

#include <vector>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::NO_INIT;

using ::vendor::lineage::livedisplay::V1_0::IColor;

class SDM : public ColorBackend {
  public:
    virtual status_t getDisplayModes(std::vector<sp<disp_mode>>& profiles) override;
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

    virtual status_t getColorBalanceRange(Range& /* range */) override {
        return NO_INIT;
    }
    virtual int32_t getColorBalance() override {
        return 0;
    }
    virtual status_t setColorBalance(int32_t /* balance */) override {
        return NO_INIT;
    }

    virtual status_t getPictureAdjustmentRanges(HSICRanges& ranges) override;
    virtual status_t setPictureAdjustment(const HSIC& hsic) override;
    virtual status_t getPictureAdjustment(HSIC& hsic) override;
    virtual HSIC getDefaultPictureAdjustment() override;

    virtual bool hasFeature(Feature f);
    virtual status_t initialize();
    virtual status_t deinitialize();

  private:
    status_t saveInitialDisplayMode();
    uint32_t getNumDisplayModes();
    uint32_t getNumSDMDisplayModes();

    sp<disp_mode> getDisplayModeById(int32_t id);
    status_t setModeState(sp<disp_mode> mode, bool state);
    sp<disp_mode> getLocalSRGBMode();
    sp<disp_mode> getLocalDCIP3Mode();

    uint64_t mHandle;
    bool mFOSSEnabled;
    bool mCachedFOSSStatus;
    int32_t mActiveModeId;

    HSIC mDefaultPictureAdjustment;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_SDM_H
