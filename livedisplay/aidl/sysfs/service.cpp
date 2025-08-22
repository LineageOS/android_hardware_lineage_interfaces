/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service-sysfs"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <livedisplay/sysfs/DisplayColorCalibration.h>
#include <livedisplay/sysfs/SimpleMode.h>
#include <livedisplay/sysfs/SunlightEnhancement.h>

using ::aidl::vendor::lineage::livedisplay::AdaptiveBacklight;
using ::aidl::vendor::lineage::livedisplay::AntiFlicker;
using ::aidl::vendor::lineage::livedisplay::AutoContrast;
using ::aidl::vendor::lineage::livedisplay::ColorEnhancement;
using ::aidl::vendor::lineage::livedisplay::DisplayColorCalibration;
using ::aidl::vendor::lineage::livedisplay::ReadingEnhancement;
using ::aidl::vendor::lineage::livedisplay::SunlightEnhancement;

int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    std::shared_ptr<AdaptiveBacklight> ab =
            ENABLE_AB ? ndk::SharedRefBase::make<AdaptiveBacklight>() : nullptr;
    std::shared_ptr<AutoContrast> ac =
            ENABLE_AC ? ndk::SharedRefBase::make<AutoContrast>() : nullptr;
    std::shared_ptr<AntiFlicker> af = ENABLE_AF ? ndk::SharedRefBase::make<AntiFlicker>() : nullptr;
    std::shared_ptr<ColorEnhancement> ce =
            ENABLE_CE ? ndk::SharedRefBase::make<ColorEnhancement>() : nullptr;
    std::shared_ptr<DisplayColorCalibration> dcc =
            ENABLE_DCC ? ndk::SharedRefBase::make<DisplayColorCalibration>() : nullptr;
    std::shared_ptr<ReadingEnhancement> re =
            ENABLE_RE ? ndk::SharedRefBase::make<ReadingEnhancement>() : nullptr;
    std::shared_ptr<SunlightEnhancement> se =
            ENABLE_SE ? ndk::SharedRefBase::make<SunlightEnhancement>() : nullptr;

    if (ab) {
        std::string instance = std::string() + AdaptiveBacklight::descriptor + "/default";
        binder_status_t status = AServiceManager_addService(ab->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    if (ac) {
        std::string instance = std::string() + AutoContrast::descriptor + "/default";
        binder_status_t status = AServiceManager_addService(ac->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    if (af) {
        std::string instance = std::string() + AntiFlicker::descriptor + "/default";
        binder_status_t status = AServiceManager_addService(af->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    if (ce) {
        std::string instance = std::string() + ColorEnhancement::descriptor + "/default";
        binder_status_t status = AServiceManager_addService(ce->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    if (dcc) {
        std::string instance = std::string() + DisplayColorCalibration::descriptor + "/default";
        binder_status_t status =
                AServiceManager_addService(dcc->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    if (re) {
        std::string instance = std::string() + ReadingEnhancement::descriptor + "/default";
        binder_status_t status = AServiceManager_addService(re->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    if (se) {
        std::string instance = std::string() + SunlightEnhancement::descriptor + "/default";
        binder_status_t status = AServiceManager_addService(se->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK);
    }

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
