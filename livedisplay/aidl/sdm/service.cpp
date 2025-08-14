/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service.sdm"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <livedisplay/sdm/DisplayModes.h>
#include <livedisplay/sdm/PictureAdjustment.h>
#include <livedisplay/sdm/SDMController.h>

using ::aidl::vendor::lineage::livedisplay::sdm::DisplayModes;
using ::aidl::vendor::lineage::livedisplay::sdm::PictureAdjustment;
using ::aidl::vendor::lineage::livedisplay::sdm::SDMController;

int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();
    std::shared_ptr<DisplayModes> dm = ndk::SharedRefBase::make<DisplayModes>(controller);
    std::shared_ptr<PictureAdjustment> pa = ndk::SharedRefBase::make<PictureAdjustment>(controller);

    std::string instance = std::string() + DisplayModes::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(dm->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    instance = std::string() + PictureAdjustment::descriptor + "/default";
    status = AServiceManager_addService(pa->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
