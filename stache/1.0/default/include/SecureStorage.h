/*
 * Copyright (C) 2018 The LineageOS Project
 * Author: Rashed Abdel-Tawab (rashed@linux.com)
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

#ifndef VENDOR_LINEAGE_STACHE_V1_0_SECURESTORAGE_H
#define VENDOR_LINEAGE_STACHE_V1_0_SECURESTORAGE_H

#include <vendor/lineage/stache/1.0/ISecureStorage.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace stache {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct SecureStorage : public ISecureStorage {
    // Methods from ISecureStorage follow.
    Return<int32_t> getContainerStatus(const hidl_string& dirPath) override;
    Return<bool> createContainer(const hidl_string& dirPath, const CryptOptions& options) override;
    Return<bool> attachContainer(const hidl_string& dirPath, const CryptOptions& options) override;
    Return<bool> detachContainer(const hidl_string& dirPath) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" ISecureStorage* HIDL_FETCH_ISecureStorage(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace stache
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_STACHE_V1_0_SECURESTORAGE_H
