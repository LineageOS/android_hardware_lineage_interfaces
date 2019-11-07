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

#define LOG_TAG "vendor.qti.hardware.cryptfshw@1.0-impl-qti.kernel"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/unique_fd.h>
#include <linux/qseecom.h>
#include <Types.h>

#include "KernelController.h"

namespace vendor {
namespace qti {
namespace hardware {
namespace cryptfshw {
namespace V1_0 {
namespace implementation {
namespace kernel {

using ::android::base::unique_fd;

int Controller::createKey(int usage, unsigned char* hash32) {
    qseecom_create_key_req req;
    int32_t ret;

    if (usage < CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION || usage > CRYPTFS_HW_KM_USAGE_MAX) {
        LOG_TO(SYSTEM, ERROR) << "Error:: unsupported usage " << usage;
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    unique_fd qseecom_fd(open("/dev/qseecom", O_RDWR));
    if (qseecom_fd < 0) {
        LOG_TO(SYSTEM, ERROR) << "Error::Failed to open /dev/qseecom device";
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    req.usage = static_cast<qseecom_key_management_usage_type>(usage);
    memcpy((void*)req.hash32, (void*)hash32, QSEECOM_HASH_SIZE);

    ret = ioctl(qseecom_fd, QSEECOM_IOCTL_CREATE_KEY_REQ, &req);
    if (ret) {
        LOG_TO(SYSTEM, ERROR) << "Error::ioctl call to create encryption key for usage " << usage
                              << " failed with ret = " << ret << ", errno = " << errno;
        if (errno == ERANGE)
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        else
            ret = CRYPTFS_HW_CREATE_KEY_FAILED;
    } else {
        LOG_TO(SYSTEM, ERROR) << "SUCESS::ioctl call to create encryption key for usage " << usage
                              << " success with ret = " << ret;
    }
    return ret;
}

int Controller::updateKey(int usage, unsigned char* current_hash32, unsigned char* new_hash32) {
    qseecom_update_key_userinfo_req req;
    int32_t ret;

    if (usage < CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION || usage > CRYPTFS_HW_KM_USAGE_MAX) {
        LOG_TO(SYSTEM, ERROR) << "Error:: unsupported usage " << usage;
        return CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    unique_fd qseecom_fd(open("/dev/qseecom", O_RDWR));
    if (qseecom_fd < 0) {
        LOG_TO(SYSTEM, ERROR) << "Error::Failed to open /dev/qseecom device";
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    req.usage = static_cast<qseecom_key_management_usage_type>(usage);
    memcpy((void*)req.current_hash32, (void*)current_hash32, QSEECOM_HASH_SIZE);
    memcpy((void*)req.new_hash32, (void*)new_hash32, QSEECOM_HASH_SIZE);

    ret = ioctl(qseecom_fd, QSEECOM_IOCTL_UPDATE_KEY_USER_INFO_REQ, &req);
    if (ret) {
        LOG_TO(SYSTEM, ERROR) << "Error::ioctl call to update the encryption key for usage "
                              << usage << " failed with ret = " << ret << ", errno = " << errno;
        if (errno == ERANGE)
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        else
            ret = CRYPTFS_HW_UPDATE_KEY_FAILED;
    } else {
        LOG_TO(SYSTEM, ERROR) << "SUCCESS::Qseecom call to update the encryption key for usage "
                              << usage << " success with ret = " << ret;
    }
    return ret;
}

int Controller::wipeKey(int usage) {
    qseecom_wipe_key_req req;
    int32_t ret;

    if (usage < CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION || usage > CRYPTFS_HW_KM_USAGE_MAX) {
        LOG_TO(SYSTEM, ERROR) << "Error:: unsupported usage " << usage;
        return CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    unique_fd qseecom_fd(open("/dev/qseecom", O_RDWR));
    if (qseecom_fd < 0) {
        LOG_TO(SYSTEM, ERROR) << "Error::Failed to open /dev/qseecom device";
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    req.usage = static_cast<qseecom_key_management_usage_type>(usage);
    req.wipe_key_flag = CRYPTFS_HW_KMS_WIPE_KEY;

    ret = ioctl(qseecom_fd, QSEECOM_IOCTL_WIPE_KEY_REQ, &req);
    if (ret) {
        LOG_TO(SYSTEM, ERROR) << "Error::ioctl call to wipe the encryption key for usage " << usage
                              << " failed with ret = " << ret << ", errno = " << errno;
        ret = CRYPTFS_HW_WIPE_KEY_FAILED;
    } else {
        LOG_TO(SYSTEM, ERROR) << "SUCCESS::ioctl call to wipe the encryption key for usage "
                              << usage << " success with ret = " << ret;
    }
    return ret;
}

}  // namespace kernel
}  // namespace implementation
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
