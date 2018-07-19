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

#include <sys/stat.h>
#include <sys/types.h>

#include <log/log.h>

#include "linux/fs.h"

#include "crypto-helper.h"
#include "key-helper.h"
#include "policy-helper.h"
#include "SecureStorage.h"

namespace vendor {
namespace lineage {
namespace stache {
namespace V1_0 {
namespace implementation {

// Methods from ISecureStorage follow.
Return<int32_t> SecureStorage::getContainerStatus(const hidl_string& dirPath) {
    // TODO implement
    return int32_t {};
}

Return<bool> SecureStorage::createContainer(const hidl_string& dirPath, const CryptOptions& options) {
    int errno;
    mode_t mode = 0755; // TODO: Don't hardcode this

    errno = mkdir(dirPath.c_str(), mode);
    if (errno != 0) {
         ALOGE("Failed to create container %s (%d): %s", dirPath.c_str(), errno, strerror(errno));
         return false;
    }
    // As Kernel version 4.9, the only policy field that has multiple valid
    // options is "flags", which sets the amount of zero padding on filenames.
    struct fscrypt_policy policy = {
        .version = 0,
        .contents_encryption_mode = FS_ENCRYPTION_MODE_AES_256_XTS,
        .filenames_encryption_mode = FS_ENCRYPTION_MODE_AES_256_CTS,
        // Use maximum zero-padding to leak less info about filename length
        .flags = FS_POLICY_FLAGS_PAD_32
    };
    policy.contents_encryption_mode = string_to_mode(options.contentsCipher.c_str());
    policy.filenames_encryption_mode = string_to_mode(options.filenameCipher.c_str());
    policy.flags = options.filenamePadding;

    // Copy the descriptor into the policy, requires changing format.
    if (key_descriptor_to_bytes(options.keyDescriptor.c_str(), policy.master_key_descriptor)) {
        ALOGE("Invalid descriptor: %s", options.keyDescriptor.c_str());
        return false;
    }

    errno = set_policy(dirPath.c_str(), &policy);
    if (errno != 0) {
        ALOGE("Failed to set policy for container %s (%d): %s", dirPath.c_str(), errno, strerror(errno));
        return false;
    }

    return true;
}

Return<bool> SecureStorage::attachContainer(const hidl_string& dirPath, const CryptOptions& options) {
    // TODO implement
    return bool {};
}

Return<bool> SecureStorage::detachContainer(const hidl_string& dirPath) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//ISecureStorage* HIDL_FETCH_ISecureStorage(const char* /* name */) {
//    return new SecureStorage();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace stache
}  // namespace lineage
}  // namespace vendor
