#ifndef VENDOR_LINEAGE_CONFIGSTORE_V1_0_OMXCONFIGS_H
#define VENDOR_LINEAGE_CONFIGSTORE_V1_0_OMXCONFIGS_H

#include <vendor/lineage/configstore/1.0/IOmxConfigs.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace configstore {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct OmxConfigs : public IOmxConfigs {
    // Methods from IOmxConfigs follow.
    Return<void> hasLegacyQCOMOmx(hasLegacyQCOMOmx_cb _hidl_cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IOmxConfigs* HIDL_FETCH_IOmxConfigs(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace configstore
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_CONFIGSTORE_V1_0_OMXCONFIGS_H
