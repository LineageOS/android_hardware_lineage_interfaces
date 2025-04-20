/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

 #include <aidl/vendor/lineage/powershare/IPowerShare.h>
 #include "PowerShare.h"
 #include <android/binder_process.h>
 #include <android/binder_manager.h>
 #include <android/binder_status.h>

 using aidl::vendor::lineage::powershare::IPowerShare;
 using aidl::vendor::lineage::powershare::PowerShare;

int main() {
    std::shared_ptr<IPowerShare> service = ndk::SharedRefBase::make<PowerShare>();

    // register the sevice
    const std::string instance = std::string(IPowerShare::descriptor) + "/default";
    binder_status_t status = AServiceManager_addService(
        service->asBinder().get(), instance.c_str()
    );

    if (status != STATUS_OK) {
        LOG(ERROR) << "Can't register PowerShare HAL service";
        return 1;
    }

    // keep the service alive
    ABinderProcess_joinThreadPool();

    return 0; // should never get here
}
