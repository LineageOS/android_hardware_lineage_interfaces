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

#define LOG_TAG "vendor.qti.hardware.cryptfshw@1.0-impl-qti"

#include <dlfcn.h>
#include <linux/qseecom.h>

#include <android-base/logging.h>

#include "CryptfsHwUtils.h"
#include "QSEEComController.h"
#include "Types.h"

namespace {
constexpr char kFilename[] = "libQSEEComAPI.so";

bool IsQseecomUp() {
#ifdef WAIT_FOR_QSEE
    for (size_t i = 0; i < CRYPTFS_HW_UP_CHECK_COUNT; i++) {
        if (GetBoolProperty("sys.keymaster.loaded", false)) {
			return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    LOG(ERROR) << "Timed out waiting for QSEECom";
	return false;
#else
    return true;
#endif
}
}  // anonymous namespace

namespace vendor {
namespace qti {
namespace hardware {
namespace cryptfshw {
namespace V1_0 {
namespace implementation {
namespace qti {

Controller::Controller() {
    std::shared_ptr<void> handle(dlopen(kFilename, RTLD_LAZY | RTLD_LOCAL), [this](void* p) {
        mFn_create_key = nullptr;
        mFn_update_key_user_info = nullptr;
        mFn_wipe_key = nullptr;

        if (p != nullptr) {
            int err = dlclose(p);
            p = nullptr;
            if (err != 0) {
                LOG(ERROR) << "FAILED TO CLOSE LIBRARY " << kFilename;
            }
        }
    });
    if (handle == nullptr) {
        LOG(ERROR) << "FAILED TO LOAD LIBRARY " << kFilename << ": " << dlerror();
        return;
    }

    if (!IsQseecomUp()) {
        LOG_TO(SYSTEM, ERROR)
                << "Timed out waiting for QSEECom listeners. Aborting FDE key operation";
        return;
    }

    mHandle = handle;
    mFn_create_key = loadFunction<int (*)(int, void*)>("QSEECom_create_key");
    mFn_update_key_user_info =
            loadFunction<int (*)(int, void*, void*)>("QSEECom_update_key_user_info");
    mFn_wipe_key = loadFunction<int (*)(int)>("QSEECom_wipe_key");
}

template <typename Function>
Function Controller::loadFunction(const char* name) {
    void* fn = dlsym(mHandle.get(), name);
    if (fn == nullptr) {
        LOG(ERROR) << "loadFunction -- failed to load function " << name;
    }
    return reinterpret_cast<Function>(fn);
}

int Controller::createKey(int usage, unsigned char* hash32) {
    qseecom_create_key_req req;
    int32_t ret;

    if (mFn_create_key == nullptr) return CRYPTFS_HW_UPDATE_KEY_FAILED;

    if (usage < CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION || usage >= CRYPTFS_HW_KM_USAGE_MAX) {
        LOG_TO(SYSTEM, ERROR) << "Error:: unsupported usage " << usage;
        return CRYPTFS_HW_CREATE_KEY_FAILED;
    }

    if (!hash32) {
        secure_memset((void*)req.hash32, 0, QSEECOM_HASH_SIZE);
    } else {
        memscpy((void*)req.hash32, QSEECOM_HASH_SIZE, (void*)hash32, QSEECOM_HASH_SIZE);
    }

    req.usage = static_cast<qseecom_key_management_usage_type>(usage);
    ret = mFn_create_key(req.usage, req.hash32);
    if (ret) {
        LOG_TO(SYSTEM, ERROR) << "Error::Qseecom call to create encryption key for usage " << usage
                              << " failed with ret = " << ret << ", errno = " << errno;
        if (errno == ERANGE)
            ret = CRYPTFS_HW_KMS_MAX_FAILURE;
        else
            ret = CRYPTFS_HW_CREATE_KEY_FAILED;
    } else {
        LOG_TO(SYSTEM, ERROR) << "SUCESS::Qseecom call to create encryption key for usage " << usage
                              << " success with ret = " << ret;
    }
    return ret;
}

int Controller::updateKey(int usage, unsigned char* current_hash32, unsigned char* new_hash32) {
    qseecom_update_key_userinfo_req req;
    int32_t ret;

    if (mFn_update_key_user_info == nullptr) return CRYPTFS_HW_UPDATE_KEY_FAILED;

    if (usage < CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION || usage >= CRYPTFS_HW_KM_USAGE_MAX) {
        LOG_TO(SYSTEM, ERROR) << "Error:: unsupported usage " << usage;
        return CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    req.usage = static_cast<qseecom_key_management_usage_type>(usage);
    if (!current_hash32) {
        secure_memset((void*)req.current_hash32, 0, QSEECOM_HASH_SIZE);
    } else {
        memscpy((void*)req.current_hash32, QSEECOM_HASH_SIZE, (void*)current_hash32,
                QSEECOM_HASH_SIZE);
    }
    if (!new_hash32) {
        secure_memset((void*)req.new_hash32, 0, QSEECOM_HASH_SIZE);
    } else {
        memscpy((void*)req.new_hash32, QSEECOM_HASH_SIZE, (void*)new_hash32, QSEECOM_HASH_SIZE);
    }

    ret = mFn_update_key_user_info(req.usage, req.current_hash32, req.new_hash32);
    if (ret) {
        LOG_TO(SYSTEM, ERROR) << "Error::Qseecom call to update the encryption key for usage "
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
    int32_t ret;
    struct qseecom_wipe_key_req req;

    if (mFn_wipe_key == nullptr) return CRYPTFS_HW_UPDATE_KEY_FAILED;

    if (usage < CRYPTFS_HW_KM_USAGE_DISK_ENCRYPTION || usage >= CRYPTFS_HW_KM_USAGE_MAX) {
        LOG_TO(SYSTEM, ERROR) << "Error:: unsupported usage " << usage;
        return CRYPTFS_HW_UPDATE_KEY_FAILED;
    }

    req.usage = static_cast<qseecom_key_management_usage_type>(usage);

    ret = mFn_wipe_key(req.usage);
    if (ret) {
        LOG_TO(SYSTEM, ERROR) << "Error::Qseecom call to wipe the encryption key for usage "
                              << usage << " failed with ret = " << ret << ", errno = " << errno;
        ret = CRYPTFS_HW_WIPE_KEY_FAILED;
    } else {
        LOG_TO(SYSTEM, ERROR) << "SUCCESS::Qseecom call to wipe the encryption key for usage "
                              << usage << " success with ret = " << ret;
    }
    return ret;
}

}  // namespace qti
}  // namespace implementation
}  // namespace V1_0
}  // namespace cryptfshw
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
