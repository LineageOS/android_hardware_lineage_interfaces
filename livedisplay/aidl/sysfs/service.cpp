/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service-sysfs"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <livedisplay/sysfs/DisplayColorCalibration.h>
#include <livedisplay/sysfs/DualStateMode.h>
#include <livedisplay/sysfs/SunlightEnhancement.h>

using ::aidl::vendor::lineage::livedisplay::sysfs::AdaptiveBacklight;
using ::aidl::vendor::lineage::livedisplay::sysfs::AntiFlicker;
using ::aidl::vendor::lineage::livedisplay::sysfs::AutoContrast;
using ::aidl::vendor::lineage::livedisplay::sysfs::ColorEnhancement;
using ::aidl::vendor::lineage::livedisplay::sysfs::DisplayColorCalibration;
using ::aidl::vendor::lineage::livedisplay::sysfs::ReadingEnhancement;
using ::aidl::vendor::lineage::livedisplay::sysfs::SunlightEnhancement;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    auto ab = ENABLE_AB ? ndk::SharedRefBase::make<AdaptiveBacklight>() : nullptr;
    auto ac = ENABLE_AC ? ndk::SharedRefBase::make<AutoContrast>() : nullptr;
    auto af = ENABLE_AF ? ndk::SharedRefBase::make<AntiFlicker>() : nullptr;
    auto ce = ENABLE_CE ? ndk::SharedRefBase::make<ColorEnhancement>() : nullptr;
    auto dcc = ENABLE_DCC ? ndk::SharedRefBase::make<DisplayColorCalibration>() : nullptr;
    auto re = ENABLE_RE ? ndk::SharedRefBase::make<ReadingEnhancement>() : nullptr;
    auto se = ENABLE_SE ? ndk::SharedRefBase::make<SunlightEnhancement>() : nullptr;

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
