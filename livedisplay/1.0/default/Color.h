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
#include <vendor/lineage/livedisplay/1.0/IColor.h>

#define SUCCESS 1
#define FAIL 0

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::Mutex;
using ::android::status_t;
using ::vendor::lineage::livedisplay::V1_0::IColor;

class Color : public IColor
{
public:
    Return<Features> native_getSupportedFeatures() override;

    Color();
    ~Color();

private:
    bool connect();
    void reset();

protected:
    uint32_t mFeatures;
    bool mConnected;

    bool check(Feature f) {
        return (mFeatures & (uint32_t)f) && connect();
    }
    void error(const char* msg = NULL, ...);

    virtual bool backendHasFeature(Feature f) = 0;
    virtual status_t initialize() = 0;
    virtual status_t deinitialize() = 0;

    void addFeature(Feature f) {
        mFeatures |= (uint32_t)f;
    };

    Mutex mLock;
};

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor

#endif // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H
