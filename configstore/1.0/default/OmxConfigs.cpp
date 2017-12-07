#include "OmxConfigs.h"

namespace vendor {
namespace lineage {
namespace configstore {
namespace V1_0 {
namespace implementation {

// Methods from IOmxConfigs follow.
Return<void> OmxConfigs::hasLegacyQCOMOmx(hasLegacyQCOMOmx_cb _hidl_cb) {
#ifdef QCOM_BSP_LEGACY
    _hidl_cb({true /* specified */, true /* value */}); // TODO implement
#else
    // when macro not defined, we can give any value to the second argument.}
    // It will simply be ignored in the framework side.
    _hidl_cb({false /* specified */, false /* value */}); // Methods from ::android::hidl::base::V1_0::IBase follow.
#endif
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace configstore
}  // namespace lineage
}  // namespace vendor
