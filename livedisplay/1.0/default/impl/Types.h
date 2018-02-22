


#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_TYPES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_TYPES_H

#include <string>
#include <utils/RefBase.h>
#include <vendor/lineage/livedisplay/1.0/IColor.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using std::string;

struct disp_mode : public android::RefBase {
    int32_t id;
    string name;

    uint32_t privFlags;
    string privData;

    disp_mode() : id(-1) {}
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
    HSICRanges(Range _hue, FloatRange _saturation, FloatRange _intensity,
            FloatRange _contrast, FloatRange _saturationThreshold)
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
        return isNonZero(hue) && isNonZero(saturation) &&
               isNonZero(intensity) && isNonZero(contrast);
    }
};

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor

#endif // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_TYPES_H
