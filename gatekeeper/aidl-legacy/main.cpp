/*
 * SPDX-FileCopyrightText: 2014-2019 The Android Open Source Project
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.gatekeeper-service.legacy"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include "Gatekeeper.h"

using aidl::android::hardware::gatekeeper::Gatekeeper;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<Gatekeeper> gatekeeper = ndk::SharedRefBase::make<Gatekeeper>();

    const std::string instance = std::string() + Gatekeeper::descriptor + "/default";
    binder_status_t status =
            AServiceManager_addService(gatekeeper->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();

    return -1;  // Should never get here.
}
