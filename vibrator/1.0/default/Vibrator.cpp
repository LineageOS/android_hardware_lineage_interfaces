#include "Vibrator.h"

namespace vendor {
namespace lineage {
namespace vibrator {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
Return<Status> Vibrator::on(uint32_t timeoutMs) {
    // TODO implement
    return ::android::hardware::vibrator::V1_0::Status {};
}

Return<Status> Vibrator::off() {
    // TODO implement
    return ::android::hardware::vibrator::V1_0::Status {};
}

Return<bool> Vibrator::supportsAmplitudeControl() {
    // TODO implement
    return bool {};
}

Return<Status> Vibrator::setAmplitude(uint8_t amplitude) {
    // TODO implement
    return ::android::hardware::vibrator::V1_0::Status {};
}

Return<void> Vibrator::perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    // TODO implement
    return Void();
}


// Methods from ::vendor::lineage::vibrator::V1_0::IVibrator follow.
Return<void> Vibrator::getDefaultAmplitude(getDefaultAmplitude_cb _hidl_cb) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IVibrator* HIDL_FETCH_IVibrator(const char* /* name */) {
    return new Vibrator();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace lineage
}  // namespace vendor
