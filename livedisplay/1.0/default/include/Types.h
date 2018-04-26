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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_TYPES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_TYPES_H

#include <utils/RefBase.h>
#include <vendor/lineage/livedisplay/1.0/IColor.h>
#include <string>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

struct disp_mode : public android::RefBase {
    int32_t id;
    std::string name;

    uint32_t privFlags;
    std::string privData;

    disp_mode() : id(-1) {
    }
};

inline bool isNonZero(Range r) {
    return r.min != 0 || r.max != 0;
}

inline bool isNonZero(FloatRange r) {
    return r.min != 0.f || r.max != 0.f;
}

struct HSICRanges {
    HSICRanges() {
    }
    HSICRanges(Range _hue, FloatRange _saturation, FloatRange _intensity, FloatRange _contrast,
               FloatRange _saturationThreshold)
        : hue(_hue),
          saturation(_saturation),
          intensity(_intensity),
          contrast(_contrast),
          saturationThreshold(_saturationThreshold) {
    }

    Range hue;
    FloatRange saturation;
    FloatRange intensity;
    FloatRange contrast;
    FloatRange saturationThreshold;

    bool isValid() {
        return isNonZero(hue) && isNonZero(saturation) && isNonZero(intensity) &&
               isNonZero(contrast);
    }
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_TYPES_H
