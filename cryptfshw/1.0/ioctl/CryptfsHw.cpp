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

#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <linux/qseecom.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/properties.h>

#include "CryptfsHw.h"

using android::base::GetProperty;

namespace vendor {
namespace qti {
namespace hardware {
namespace cryptfshw {
namespace V1_0 {
namespace ioctl_qti {

CryptfsHw::CryptfsHw() {
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
Return<int32_t> CryptfsHw::setIceParam(uint32_t flag) {
    int fd = 0, ret = 0;
    qseecom_ice_data_t ice_data{};

    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) return fd;

    ice_data.flag = flag;
    ret = ioctl(fd, QSEECOM_IOCTL_SET_ICE_INFO, &ice_data);

    close(fd);

    return ret;
}

Return<int32_t> CryptfsHw::setKey(const hidl_string& passwd, const hidl_string&) {
    int fd = 0, ret = 0;
    qseecom_create_key_req req{};

    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) return CRYPTFS_HW_CREATE_KEY_FAILED;

    req.usage = static_cast<qseecom_key_management_usage_type>(mStorageType + 2);

    std::strncpy(reinterpret_cast<char*>(req.hash32), passwd.c_str(), QSEECOM_HASH_SIZE);

    ret = ioctl(fd, QSEECOM_IOCTL_CREATE_KEY_REQ, &req);
    if (ret) {
        if (errno == ERANGE)
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        else
            ret = CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    close(fd);

    return ret;
}

Return<int32_t> CryptfsHw::updateKey(const hidl_string& oldpw, const hidl_string& newpw,
                                     const hidl_string&) {
    int fd = 0, ret = 0;
    qseecom_update_key_userinfo_req req{};

    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) return CRYPTFS_HW_UPDATE_KEY_FAILED;

    req.usage = static_cast<qseecom_key_management_usage_type>(mStorageType + 2);

    std::strncpy(reinterpret_cast<char*>(req.current_hash32), oldpw.c_str(), QSEECOM_HASH_SIZE);
    std::strncpy(reinterpret_cast<char*>(req.new_hash32), newpw.c_str(), QSEECOM_HASH_SIZE);

    ret = ioctl(fd, QSEECOM_IOCTL_UPDATE_KEY_USER_INFO_REQ, &req);
    if (ret) {
        if (errno == ERANGE)
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        else
            ret = CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    close(fd);

    return ret;
}

Return<int32_t> CryptfsHw::clearKey() {
    int fd = 0, ret = 0;
    qseecom_wipe_key_req req{};

    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) return CRYPTFS_HW_WIPE_KEY_FAILED;

    req.usage = static_cast<qseecom_key_management_usage_type>(mStorageType + 2);
    req.wipe_key_flag = CRYPTFS_HW_KMS_WIPE_KEY;

    ret = ioctl(fd, QSEECOM_IOCTL_WIPE_KEY_REQ, &req);
    if (ret) ret = CRYPTFS_HW_WIPE_KEY_FAILED;

    return ret;
}

}  // namespace ioctl_qti
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
