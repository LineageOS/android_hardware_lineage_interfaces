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

#pragma once

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/qti/hardware/cryptfshw/1.0/ICryptfsHw.h>

namespace vendor {
namespace qti {
namespace hardware {
namespace cryptfshw {
namespace V1_0 {
namespace ioctl_qti {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

#define QTI_ICE_STORAGE_NOT -1
#define QTI_ICE_STORAGE_UFS 1
#define QTI_ICE_STORAGE_SDCC 2

#define CRYPTFS_HW_KMS_WIPE_KEY 1
#define CRYPTFS_HW_CREATE_KEY_FAILED -7
#define CRYPTFS_HW_WIPE_KEY_FAILED -8
#define CRYPTFS_HW_UPDATE_KEY_FAILED -9
#define CRYPTFS_HW_KMS_MAX_FAILURE -10

enum cryptfs_hw_key_management_usage_type {
    CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION = 0x01,
    CRYPTFS_HW_KM_USAGE_FILE_ENCRYPTION = 0x02,
    CRYPTFS_HW_KM_USAGE_UFS_ICE_DISK_ENCRYPTION = 0x03,
    CRYPTFS_HW_KM_USAGE_SDCC_ICE_DISK_ENCRYPTION = 0x04,
    CRYPTFS_HW_KM_USAGE_MAX
};

class CryptfsHw : public ICryptfsHw {
   public:
    CryptfsHw();

    // Methods from ::vendor::qti::hardware::cryptfshw::V1_0::ICryptfsHw follow.
    Return<int32_t> setIceParam(uint32_t flag) override;
    Return<int32_t> setKey(const hidl_string& passwd, const hidl_string&) override;
    Return<int32_t> updateKey(const hidl_string& oldpw, const hidl_string& newpw,
                              const hidl_string&) override;
    Return<int32_t> clearKey() override;

   private:
    int mStorageType;
};

}  // namespace ioctl_qti
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
