/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Charger.h"
#include "UeventListener.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <thread>

using ::aidl::vendor::lineage::chgctrl::Charger;
using ::aidl::vendor::lineage::chgctrl::UeventListener;

#define LOG_TAG "vendor.lineage.chgctrl-service.example"

int main() {
    LOG(INFO) << "Lineage ChgCtrl HAL Starting ...";
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<Charger> chg = ndk::SharedRefBase::make<Charger>();

    const std::string instance = std::string() + Charger::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(chg->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    LOG(INFO) << "Uevent listener starting ...";
    UeventListener ueventListener;
    std::thread listenThread(&UeventListener::ListenForever, &ueventListener);
    listenThread.detach();
    LOG(INFO) << "Uevent listener thread start done!";

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
