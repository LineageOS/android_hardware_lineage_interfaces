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

    // Figure out what kind of device we have to set the magic
    // constant used when talking to the backend
    if (bootdevice.find("ufs") != std::string::npos) {
        // All UFS devices use ICE
        mKeyManagementUsage = CRYPTFS_HW_KM_USAGE_UFS_ICE_DISK_ENCRYPTION;
    } else if (bootdevice.find("sdhc") != std::string::npos) {
        // Check if we're on a SDHC device that does support ICE
        if (access("/dev/icesdcc", F_OK) == 0)
            mKeyManagementUsage = CRYPTFS_HW_KM_USAGE_SDCC_ICE_DISK_ENCRYPTION;
        else
            mKeyManagementUsage = CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION;
    } else {
        // If we don't know what kind of storage we're using,
        // we can't use ICE
        mKeyManagementUsage = CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION;
    }
}

// Methods from ::vendor::qti::hardware::cryptfshw::V1_0::ICryptfsHw follow.
Return<int32_t> CryptfsHw::setIceParam(uint32_t flag) {
#ifdef QSEECOM_IOCTL_SET_ICE_INFO
    int fd = 0, ret = 0;
    qseecom_ice_data_t ice_data{};

    // Open fd to prepare for ioctl
    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) {
        LOG(ERROR) << "Unable to open qseecom fd, errno: " << errno;
        return fd;
    }

    // Setup ioctl by filling out struct
    ice_data.flag = flag;

    // Perform ioctl call
    ret = ioctl(fd, QSEECOM_IOCTL_SET_ICE_INFO, &ice_data);
    if (ret) {
        LOG(ERROR) << "ioctl QSEECOM_IOCTL_SET_ICE_INFO failed, errno: " << errno;
    }

    // Close fd to prevent resource leaks
    close(fd);

    return ret;
#else
    // Avoid unused parameter errors
    (void)flag;
    return -1;
#endif
}

Return<int32_t> CryptfsHw::setKey(const hidl_string& passwd, const hidl_string& enc_mode) {
    int fd = 0, ret = 0;
    qseecom_create_key_req req{};

    // Verify that we're in HW encryption mode
    if (enc_mode != "aes-xts") {
        LOG(DEBUG) << "Not in HW disk encryption mode";
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    // Open fd to prepare for ioctl
    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) {
        LOG(ERROR) << "Unable to open qseecom fd, errno: " << errno;
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    // Setup ioctl by filling out struct
    req.usage = mKeyManagementUsage;
    std::strncpy(reinterpret_cast<char*>(req.hash32), passwd.c_str(), QSEECOM_HASH_SIZE);

    // Perform ioctl call
    ret = ioctl(fd, QSEECOM_IOCTL_CREATE_KEY_REQ, &req);
    if (ret) {
        LOG(ERROR) << "ioctl QSEECOM_IOCTL_CREATE_KEY_REQ failed, errno: " << errno;
        if (errno == ERANGE) {
            LOG(INFO) << "Maximum number of password attempts reached, userdata will be erased";
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        } else {
            ret = CRYPTFS_HW_CREATE_KEY_FAILED;
        }
    }

    // Clear password from struct to prevent leaking password
    std::strncpy(reinterpret_cast<char*>(req.hash32), "", QSEECOM_HASH_SIZE);

    // Close fd to prevent resource leaks
    close(fd);

    return ret;
}

Return<int32_t> CryptfsHw::updateKey(const hidl_string& oldpw, const hidl_string& newpw,
                                     const hidl_string& enc_mode) {
    int fd = 0, ret = 0;
    qseecom_update_key_userinfo_req req{};

    // Verify that we're in HW encryption mode
    if (enc_mode != "aes-xts") {
        LOG(DEBUG) << "Not in HW disk encryption mode";
        return CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    // Open fd to prepare for ioctl
    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) {
        LOG(ERROR) << "Unable to open qseecom fd, errno: " << errno;
        return CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    // Setup ioctl by filling out struct
    req.usage = mKeyManagementUsage;
    std::strncpy(reinterpret_cast<char*>(req.current_hash32), oldpw.c_str(), QSEECOM_HASH_SIZE);
    std::strncpy(reinterpret_cast<char*>(req.new_hash32), newpw.c_str(), QSEECOM_HASH_SIZE);

    // Perform ioctl call
    ret = ioctl(fd, QSEECOM_IOCTL_UPDATE_KEY_USER_INFO_REQ, &req);
    if (ret) {
        LOG(ERROR) << "ioctl QSEECOM_IOCTL_UPDATE_KEY_USER_INFO_REQ failed, errno: " << errno;
        if (errno == ERANGE) {
            LOG(INFO) << "Maximum number of password attempts reached, userdata will be erased";
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        } else {
            ret = CRYPTFS_HW_UPDATE_KEY_FAILED;
        }
    }

    // Clear password from struct to prevent leaking password
    std::strncpy(reinterpret_cast<char*>(req.current_hash32), "", QSEECOM_HASH_SIZE);
    std::strncpy(reinterpret_cast<char*>(req.new_hash32), "", QSEECOM_HASH_SIZE);

    // Close fd to prevent resource leaks
    close(fd);

    return ret;
}

Return<int32_t> CryptfsHw::clearKey() {
    int fd = 0, ret = 0;
    qseecom_wipe_key_req req{};

    // Verify that we're in HW encryption mode
    if (enc_mode != "aes-xts") {
        LOG(DEBUG) << "Not in HW disk encryption mode";
        return CRYPTFS_HW_WIPE_KEY_FAILED;
    }

    // Open fd to prepare for ioctl
    fd = open("/dev/qseecom", O_RDWR);
    if (fd < 0) {
        LOG(ERROR) << "Unable to open qseecom fd, errno: " << errno;
        return CRYPTFS_HW_WIPE_KEY_FAILED;
    }

    // Setup ioctl by filling out struct
    req.usage = mKeyManagementUsage;
    req.wipe_key_flag = CRYPTFS_HW_KMS_WIPE_KEY;

    // Perform ioctl call
    ret = ioctl(fd, QSEECOM_IOCTL_WIPE_KEY_REQ, &req);
    if (ret) {
        LOG(ERROR) << "ioctl QSEECOM_IOCTL_WIPE_KEY_REQ failed, errno: " << errno;
        ret = CRYPTFS_HW_WIPE_KEY_FAILED;
    }

    return ret;
}

}  // namespace ioctl_qti
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
