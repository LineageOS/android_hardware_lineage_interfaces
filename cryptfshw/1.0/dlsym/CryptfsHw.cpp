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

#include <dlfcn.h>

#include <android-base/properties.h>

#include "CryptfsHw.h"

using android::base::GetProperty;

namespace vendor {
namespace qti {
namespace hardware {
namespace cryptfshw {
namespace V1_0 {
namespace dlsym_qti {

CryptfsHw::CryptfsHw(void* libHandle) {
    mLibHandle = libHandle;
    qseecom_create_key =
        reinterpret_cast<int (*)(int, void*)>(dlsym(mLibHandle, "qseecom_create_key"));
    qseecom_update_key =
        reinterpret_cast<int (*)(int, void*, void*)>(dlsym(mLibHandle, "qseecom_update_key"));
    qseecom_wipe_key = reinterpret_cast<int (*)(int)>(dlsym(mLibHandle, "qseecom_wipe_key"));
    std::string bootdevice = GetProperty("ro.boot.bootdevice", "");

    if (bootdevice.find("ufs") != std::string::npos) {
        mStorageType = QTI_ICE_STORAGE_UFS;
    } else if (bootdevice.find("sdhc") != std::string::npos) {
        if (access("/dev/icesdcc", F_OK) == 0)
            mStorageType = QTI_ICE_STORAGE_SDCC;
        else
            mStorageType = QTI_ICE_STORAGE_NOT;
    } else {
        mStorageType = QTI_ICE_STORAGE_NOT;
    }
}

// Methods from ::vendor::qti::hardware::cryptfshw::V1_0::ICryptfsHw follow.
Return<int32_t> CryptfsHw::setIceParam(uint32_t) {
    return -1;
}

Return<int32_t> CryptfsHw::setKey(const hidl_string& passwd, const hidl_string&) {
    if (qseecom_create_key != nullptr)
        return qseecom_create_key(mStorageType + 2, const_cast<char*>(passwd.c_str()));

    return CRYPTFS_HW_CREATE_KEY_FAILED;
}

Return<int32_t> CryptfsHw::updateKey(const hidl_string& oldpw, const hidl_string& newpw,
                                     const hidl_string&) {
    if (qseecom_update_key != nullptr)
        return qseecom_update_key(mStorageType + 2, const_cast<char*>(oldpw.c_str()),
                                  const_cast<char*>(newpw.c_str()));

    return CRYPTFS_HW_UPDATE_KEY_FAILED;
}

Return<int32_t> CryptfsHw::clearKey() {
    if (qseecom_wipe_key != nullptr) return qseecom_wipe_key(mStorageType + 2);

    return CRYPTFS_HW_WIPE_KEY_FAILED;
}

}  // namespace dlsym_qti
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
