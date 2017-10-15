#ifndef VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H
#define VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H

#include <vendor/lineage/vibrator/1.0/IVibrator.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace vibrator {
namespace V1_0 {
namespace implementation {

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::lineage::vibrator::V1_0::Effect;
using ::vendor::lineage::vibrator::V1_0::EffectStrength;
using ::vendor::lineage::vibrator::V1_0::IVibrator;
using ::vendor::lineage::vibrator::V1_0::Status;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Vibrator : public IVibrator {
    // Methods from ::vendor::lineage::vibrator::V1_0::IVibrator follow.
    Return<Status> on(uint32_t timeoutMs) override;
    Return<Status> off() override;
    Return<bool> supportsAmplitudeControl() override;
    Return<Status> setAmplitude(uint8_t amplitude) override;
    Return<void> perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IVibrator* HIDL_FETCH_IVibrator(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_VIBRATOR_V1_0_VIBRATOR_H
