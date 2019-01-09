/*
 * Copyright (C) 2019 The LineageOS Project
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
#ifndef VENDOR_LINEAGE_TRUST_V1_0_USBRESTRICT_H
#define VENDOR_LINEAGE_TRUST_V1_0_USBRESTRICT_H

#include <vendor/lineage/trust/1.0/IUsbRestrict.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace trust {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct UsbRestrict : public IUsbRestrict {
    // Methods from ::vendor::lineage::trust::V1_0::IUsbRestrict follow.
    Return<bool> isEnabled() override;
    Return<void> setEnabled(bool enabled) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IUsbRestrict* HIDL_FETCH_IUsbRestrict(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace trust
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_TRUST_V1_0_USBRESTRICT_H
