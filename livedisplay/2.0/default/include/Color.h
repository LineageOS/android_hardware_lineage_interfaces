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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLOR_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLOR_H

#include <vendor/lineage/livedisplay/2.0/IAdaptiveBacklight.h>
#include <vendor/lineage/livedisplay/2.0/IColorBalance.h>
#include <vendor/lineage/livedisplay/2.0/IDisplayModes.h>
#include <vendor/lineage/livedisplay/2.0/IPictureAdjustment.h>
#include <vendor/lineage/livedisplay/2.0/ISunlightEnhancement.h>

#include <utils/Mutex.h>

#include <memory>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

class ColorBackend;

class Color : public android::RefBase {
  public:
    Color();
    ~Color();

    bool check(Feature f);

    // IAdaptiveBacklight
    bool setAdaptiveBacklightEnabled(bool enabled);
    bool isAdaptiveBacklightEnabled();

    // IColorBalance
    void getColorBalanceRange(IColorBalance::getRange_cb _hidl_cb);
    int32_t getColorBalance();
    bool setColorBalance(int32_t value);

    // IDisplayModes
    void getDisplayModes(IDisplayModes::getModes_cb _hidl_cb);
    void getCurrentDisplayMode(IDisplayModes::getCurrentMode_cb _hidl_cb);
    void getDefaultDisplayMode(IDisplayModes::getDefaultMode_cb _hidl_cb);
    bool setDisplayMode(int32_t modeID, bool makeDefault);

    // ISunlightEnhancement
    bool setOutdoorModeEnabled(bool enabled);
    bool isOutdoorModeEnabled();

    // IPictureAdjustment
    bool setPictureAdjustment(const HSIC& hsic);
    void getPictureAdjustment(IPictureAdjustment::getAdjustment_cb _hidl_cb);
    void getDefaultPictureAdjustment(IPictureAdjustment::getDefaultAdjustment_cb _hidl_cb);
    void getHueRange(IPictureAdjustment::getHueRange_cb _hidl_cb);
    void getSaturationRange(IPictureAdjustment::getSaturationRange_cb _hidl_cb);
    void getIntensityRange(IPictureAdjustment::getIntensityRange_cb _hidl_cb);
    void getContrastRange(IPictureAdjustment::getContrastRange_cb _hidl_cb);
    void getSaturationThresholdRange(IPictureAdjustment::getSaturationThresholdRange_cb _hidl_cb);

  private:
    bool connect();
    void reset();

    uint32_t mFeatures;
    bool mConnected;

    void error(const char* msg = NULL);

    void addFeature(Feature f) {
        mFeatures |= (uint32_t)f;
    };

    uint32_t getSupportedFeatures();

    std::unique_ptr<ColorBackend> mBackend;
    android::Mutex mLock;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLOR_H
