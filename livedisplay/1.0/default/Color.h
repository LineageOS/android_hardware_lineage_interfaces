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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H

#include <utils/Mutex.h>
#include "impl/LiveDisplayBackend.h"
#include <vendor/lineage/livedisplay/1.0/IColor.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::Mutex;
using ::vendor::lineage::livedisplay::V1_0::IColor;

using ::android::NO_INIT;
using ::android::OK;

class Color : public IColor
{
public:
    static IColor* getInstance();

    Return<Features> native_getSupportedFeatures() override;

    Return<void> native_getDisplayModes(native_getDisplayModes_cb _hidl_cb) override;
    Return<void> native_getCurrentDisplayMode(native_getCurrentDisplayMode_cb _hidl_cb) override;
    Return<void> native_getDefaultDisplayMode(native_getDefaultDisplayMode_cb _hidl_cb) override;
    Return<bool> native_setDisplayMode(int32_t modeID, bool makeDefault) override;

    Return<bool> native_setAdaptiveBacklightEnabled(bool enabled) override;
    Return<bool> native_isAdaptiveBacklightEnabled() override;

    Return<bool> native_setOutdoorModeEnabled(bool enabled) override;
    Return<bool> native_isOutdoorModeEnabled() override;

    Return<void> native_getColorBalanceRange(native_getColorBalanceRange_cb _hidl_cb) override;
    Return<int32_t> native_getColorBalance() override;
    Return<bool> native_setColorBalance(int32_t value) override;

    Return<bool> native_setPictureAdjustment(const HSIC& hsic) override;
    Return<void> native_getPictureAdjustment(native_getPictureAdjustment_cb _hidl_cb) override;
    Return<void> native_getDefaultPictureAdjustment(native_getDefaultPictureAdjustment_cb _hidl_cb) override;

    Return<void> native_getHueRange(native_getHueRange_cb _hidl_cb) override;
    Return<void> native_getSaturationRange(native_getSaturationRange_cb _hidl_cb) override;
    Return<void> native_getIntensityRange(native_getIntensityRange_cb _hidl_cb) override;
    Return<void> native_getContrastRange(native_getContrastRange_cb _hidl_cb) override;
    Return<void> native_getSaturationThresholdRange(native_getSaturationThresholdRange_cb _hidl_cb) override;

    ~Color();

private:
    bool connect();
    void reset();

    Color();
    static Color* sInstance;

    uint32_t mFeatures;
    bool mConnected;

    bool check(Feature f) {
        return connect() && (mFeatures & (uint32_t)f);
    }
    void error(const char* msg = NULL, ...);

    DisplayMode modePointerToObj(sp<disp_mode> mode);
    DisplayMode invalidDisplayMode();

    void addFeature(Feature f) {
        mFeatures |= (uint32_t)f;
    };

    LiveDisplayBackend* mBackend;
    Mutex mLock;
};

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor

#endif // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H
