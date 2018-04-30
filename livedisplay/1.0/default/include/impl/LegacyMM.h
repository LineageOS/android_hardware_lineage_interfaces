/*
** Copyright 2016, The CyanogenMod Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_LEGACYMM_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_LEGACYMM_H

#include "ColorBackend.h"
#include "controller/LegacyMMController.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

class LegacyMM : public ColorBackend {
  public:
    virtual status_t setAdaptiveBacklightEnabled(bool /* enabled */) override {
        return NO_INIT;
    }

    virtual bool isAdaptiveBacklightEnabled() override {
        return false;
    }

    virtual status_t setOutdoorModeEnabled(bool /* enabled */) override {
        return NO_INIT;
    }

    virtual bool isOutdoorModeEnabled() override {
        return false;
    }

    virtual status_t getColorBalanceRange(Range& range) override;
    virtual status_t setColorBalance(int32_t balance) override;
    virtual int32_t getColorBalance() override;

    virtual status_t getDisplayModes(std::vector<sp<disp_mode>>& profiles) override;
    virtual status_t setDisplayMode(int32_t modeID, bool makeDefault) override;
    virtual sp<disp_mode> getCurrentDisplayMode() override;
    virtual sp<disp_mode> getDefaultDisplayMode() override;

    virtual status_t getPictureAdjustmentRanges(HSICRanges& ranges) override;
    virtual status_t getPictureAdjustment(HSIC& hsic) override;
    virtual HSIC getDefaultPictureAdjustment() override;
    virtual status_t setPictureAdjustment(const HSIC& hsic) override;

    virtual status_t initialize() override;
    virtual status_t deinitialize() override;
    virtual bool hasFeature(Feature feature) override;

  private:
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
