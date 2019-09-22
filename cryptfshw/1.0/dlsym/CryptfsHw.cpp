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
#include <dlfcn.h>
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
namespace dlsym_qti {

CryptfsHw::CryptfsHw(void* libHandle) {
    // Store reference to dlopen()'ed lib
    // This works around refcounting in dlopen()
    mLibHandle = libHandle;
    // dlsym() the needed functions from libQSEEComAPI
    qseecom_create_key =
        reinterpret_cast<int (*)(int, void*)>(dlsym(mLibHandle, "qseecom_create_key"));
    qseecom_update_key =
        reinterpret_cast<int (*)(int, void*, void*)>(dlsym(mLibHandle, "qseecom_update_key"));
    qseecom_wipe_key = reinterpret_cast<int (*)(int)>(dlsym(mLibHandle, "qseecom_wipe_key"));
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
    int ret = CRYPTFS_HW_CREATE_KEY_FAILED;
    unsigned char tmp[QSEECOM_HASH_SIZE]{};

    // Verify that we're in HW encryption mode
    if (enc_mode != "aes-xts") {
        LOG(DEBUG) << "Not in HW disk encryption mode";
        return ret;
    }

    // Prepare for call to dlsym()'ed function to avoid having to use a
    // const_cast on passwd.c_str(), just in case the qseecom lib makes
    // changes to the buffer that we aren't expecting
    std::strncpy(reinterpret_cast<char*>(tmp), passwd.c_str(), QSEECOM_HASH_SIZE);

    // Call the function from the backend
    if (qseecom_create_key != nullptr)
        ret = qseecom_create_key(mKeyManagementUsage, tmp);
    else
        LOG(ERROR) << "qseecom_create_key not found with dlsym() in libQSEEComAPI";

    // Clear password from temporary buffer to prevent leaking the password
    std::strncpy(reinterpret_cast<char*>(tmp), "", QSEECOM_HASH_SIZE);

    return ret;
}

Return<int32_t> CryptfsHw::updateKey(const hidl_string& oldpw, const hidl_string& newpw,
                                     const hidl_string& enc_mode) {
    int ret = CRYPTFS_HW_UPDATE_KEY_FAILED;
    unsigned char tmpOld[QSEECOM_HASH_SIZE]{};
    unsigned char tmpNew[QSEECOM_HASH_SIZE]{};

    // Verify that we're in HW encryption mode
    if (enc_mode != "aes-xts") {
        LOG(DEBUG) << "Not in HW disk encryption mode";
        return ret;
    }

    // Prepare for call to dlsym()'ed function to avoid having to use a
    // const_cast on {old,new}pw.c_str(), just in case the qseecom lib
    // makes changes to the buffer that we aren't expecting
    std::strncpy(reinterpret_cast<char*>(tmpOld), passwd.c_str(), QSEECOM_HASH_SIZE);
    std::strncpy(reinterpret_cast<char*>(tmpNew), passwd.c_str(), QSEECOM_HASH_SIZE);

    // Call the function from the backend
    if (qseecom_update_key != nullptr)
        ret = qseecom_update_key(mKeyManagementUsage, tmpOld, tmpNew);
    else
        LOG(ERROR) << "qseecom_update_key not found with dlsym() in libQSEEComAPI";

    // Clear password from temporary buffers to prevent leaking the password
    std::strncpy(reinterpret_cast<char*>(tmpOld), "", QSEECOM_HASH_SIZE);
    std::strncpy(reinterpret_cast<char*>(tmpNew), "", QSEECOM_HASH_SIZE);

    return ret;
}

Return<int32_t> CryptfsHw::clearKey() {
    int ret = CRYPTFS_HW_WIPE_KEY_FAILED;

    // Verify that we're in HW encryption mode
    if (enc_mode != "aes-xts") {
        LOG(DEBUG) << "Not in HW disk encryption mode";
        return ret;
    }

    // Call the function from the backend
    if (qseecom_wipe_key != nullptr)
        ret = qseecom_wipe_key(mKeyManagementUsage);
    else
        LOG(ERROR) << "qseecom_wipe_key not found with dlsym() in libQSEEComAPI";

    return ret;
}

}  // namespace dlsym_qti
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
