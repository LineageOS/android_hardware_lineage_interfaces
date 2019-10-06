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

#define LOG_TAG "vendor.qti.hardware.cryptfshw@1.0-impl-qti.lineage"

#include <dlfcn.h>

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/unique_fd.h>

#include "CryptfsHw.h"
#include "CryptfsHwUtils.h"
#include "Types.h"

using ::android::base::GetProperty;
using ::android::base::unique_fd;

namespace {
/*
 * When device comes up or when user tries to change the password, user can
 * try wrong password upto a certain number of times. If user enters wrong
 * password further, HW would wipe all disk encryption related crypto data
 * and would return an error ERR_MAX_PASSWORD_ATTEMPTS to VOLD. VOLD would
 * wipe userdata partition once this error is received.
 */
constexpr auto ERR_MAX_PASSWORD_ATTEMPTS = -10;
constexpr auto MAX_PASSWORD_LEN = 32;
constexpr auto QTI_ICE_STORAGE_UFS = 1;
constexpr auto QTI_ICE_STORAGE_SDCC = 2;
}  // anonymous namespace

namespace vendor {
namespace qti {
namespace hardware {
namespace cryptfshw {
namespace V1_0 {
namespace implementation {

static bool IsHwDiskEncryption(const hidl_string& encryption_mode) {
    if (encryption_mode == "aes-xts") {
        LOG_TO(SYSTEM, DEBUG) << "HW based disk encryption is enabled";
        return true;
    }
    return false;
}

static void GetTmpPasswd(const char* passwd, unsigned char* tmp_passwd) {
    int passwd_len = 0;

    secure_memset(tmp_passwd, 0, MAX_PASSWORD_LEN);
    if (passwd) {
        passwd_len = strnlen(passwd, MAX_PASSWORD_LEN);
        memcpy(tmp_passwd, passwd, passwd_len);
    } else {
        LOG_TO(SYSTEM, ERROR) << __func__ << ": Passed argument is NULL";
    }
}

CryptfsHw::CryptfsHw() {
    std::string bootdevice = GetProperty("ro.boot.bootdevice", "");

    if (bootdevice.find("ufs") != std::string::npos) {
        /*
         * All UFS based devices has ICE in it. So we dont need
         * to check if corresponding device exists or not
         */
        mStorageType = QTI_ICE_STORAGE_UFS;
    } else if (bootdevice.find("sdhc") != std::string::npos) {
        if (access("/dev/icesdcc", F_OK) != -1) mStorageType = QTI_ICE_STORAGE_SDCC;
    }

    mController = std::make_unique<qti::Controller>();
    if (mController == nullptr) {
        LOG(ERROR) << "Failed to create Controller";
    }
}

int CryptfsHw::mapUsage(int usage) {
    if (usage == CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION) {
        if (mStorageType == QTI_ICE_STORAGE_UFS) {
            return CRYPTFS_HW_KM_USAGE_UFS_ICE_DISK_ENCRYPTION;
        } else if (mStorageType == QTI_ICE_STORAGE_SDCC) {
            return CRYPTFS_HW_KM_USAGE_SDCC_ICE_DISK_ENCRYPTION;
        }
    }
    return usage;
}

// Methods from ::vendor::qti::hardware::cryptfshw::V1_0::ICryptfsHw follow.
#ifdef QSEECOM_IOCTL_SET_ICE_INFO
Return<int32_t> CryptfsHw::setIceParam(uint32_t flag) {
    int32_t ret = -1;
    qseecom_ice_data_t ice_data;
    unique_fd qseecom_fd(open("/dev/qseecom", O_RDWR));
    if (qseecom_fd < 0) return ret;
    ice_data.flag = static_cast<int>(flag);
    ret = ioctl(qseecom_fd, QSEECOM_IOCTL_SET_ICE_INFO, &ice_data);
    return ret;
}
#else
Return<int32_t> CryptfsHw::setIceParam(uint32_t) {
    return -1;
}
#endif

Return<int32_t> CryptfsHw::setKey(const hidl_string& passwd, const hidl_string& enc_mode) {
    int err = -1;
    unsigned char tmp_passwd[MAX_PASSWORD_LEN];

    if (mController == nullptr) return CRYPTFS_HW_CREATE_KEY_FAILED;

    if (!IsHwDiskEncryption(enc_mode)) return err;

    GetTmpPasswd(passwd.c_str(), tmp_passwd);

    err = mController->createKey(mapUsage(CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION), tmp_passwd);
    if (err < 0) {
        if (ERR_MAX_PASSWORD_ATTEMPTS == err)
            LOG_TO(SYSTEM, INFO) << "Maximum wrong password attempts reached, will erase userdata";
    }
    secure_memset(tmp_passwd, 0, MAX_PASSWORD_LEN);

    return err;
}

Return<int32_t> CryptfsHw::updateKey(const hidl_string& oldpw, const hidl_string& newpw,
                                     const hidl_string& enc_mode) {
    int err = -1;
    unsigned char tmp_passwd[MAX_PASSWORD_LEN], tmp_currentpasswd[MAX_PASSWORD_LEN];

    if (mController == nullptr) return CRYPTFS_HW_UPDATE_KEY_FAILED;

    if (!IsHwDiskEncryption(enc_mode)) return err;

    GetTmpPasswd(newpw.c_str(), tmp_passwd);
    GetTmpPasswd(oldpw.c_str(), tmp_currentpasswd);

    err = mController->updateKey(mapUsage(CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION), tmp_currentpasswd,
                                 tmp_passwd);
    if (err < 0) {
        if (ERR_MAX_PASSWORD_ATTEMPTS == err)
            LOG_TO(SYSTEM, INFO) << "Maximum wrong password attempts reached, will erase userdata";
    }
    secure_memset(tmp_currentpasswd, 0, MAX_PASSWORD_LEN);
    secure_memset(tmp_passwd, 0, MAX_PASSWORD_LEN);

    return err;
}

Return<int32_t> CryptfsHw::clearKey() {
    if (mController == nullptr) return CRYPTFS_HW_WIPE_KEY_FAILED;

    return mController->wipeKey(mapUsage(CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION));
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
