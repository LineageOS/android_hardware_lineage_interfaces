/*
 * Copyright (C) 2020 The Android Open Source Project
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

#define LOG_TAG "powerhal-libperfmgr"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android/binder_ibinder_platform.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <perfmgr/HintManager.h>
#include <processgroup/processgroup.h>

#include <thread>

#include "HintManagerQti.h"
#include "MetricUploader.h"
#include "Power.h"
#include "PowerExt.h"
#include "utils/ThermalStateListener.h"

using aidl::android::hardware::power::IPower;
using aidl::google::hardware::power::impl::pixel::MetricUploader;
using aidl::google::hardware::power::impl::pixel::Power;
using aidl::google::hardware::power::impl::pixel::PowerExt;
using aidl::google::hardware::power::impl::pixel::ThermalStateListener;
using ::aidl::lineage::hardware::power::impl::qti::HintManagerQti;
using ::android::perfmgr::HintManager;

constexpr std::string_view kPowerHalInitProp("vendor.powerhal.init");
constexpr std::string_view kPowerHalBackendProp("ro.vendor.powerhal.backend");

template <class HintManagerT>
int RunService() {
    // Parse config but do not start the looper
    HintManagerT *hm = HintManagerT::GetInstance();
    if (!hm) {
        LOG(FATAL) << "HintManager Init failed";
    }

    // set task profile "PreferIdle" to lower scheduling latency.
    if (!SetTaskProfiles(0, {"PreferIdleSet"})) {
        LOG(WARNING) << "Device does not support 'PreferIdleSet' task profile.";
    }

    // single thread
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    // core service
    auto pw = ndk::SharedRefBase::make<Power<HintManagerT>>();
    ndk::SpAIBinder pwBinder = pw->asBinder();
    AIBinder_setMinSchedulerPolicy(pwBinder.get(), SCHED_NORMAL, -20);

    // extension service
    auto pwExt = ndk::SharedRefBase::make<PowerExt<HintManagerT>>();
    auto pwExtBinder = pwExt->asBinder();
    AIBinder_setMinSchedulerPolicy(pwExtBinder.get(), SCHED_NORMAL, -20);

    // attach the extension to the same binder we will be registering
    CHECK(STATUS_OK == AIBinder_setExtension(pwBinder.get(), pwExt->asBinder().get()));

    const std::string instance = std::string() + IPower::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(pw->asBinder().get(), instance.c_str());
    CHECK(status == STATUS_OK);
    LOG(INFO) << "Lineage Power HAL AIDL Service with Extension is started.";

    std::thread initThread([&]() {
        ::android::base::WaitForProperty(kPowerHalInitProp.data(), "1");
        HintManagerT::GetInstance()->Start();
        MetricUploader::getInstance()->init();
        ThermalStateListener::getInstance()->init();
    });
    initThread.detach();

    ABinderProcess_joinThreadPool();

    // should not reach
    LOG(ERROR) << "Lineage Power HAL AIDL Service with Extension just died.";
    return EXIT_FAILURE;
}

int main() {
    android::base::SetDefaultTag(LOG_TAG);
    android::base::SetMinimumLogSeverity(android::base::INFO);

    const std::string backend = ::android::base::GetProperty(kPowerHalBackendProp.data(), "");
    if (backend == "qti") {
        return RunService<HintManagerQti>();
    }
    return RunService<HintManager>();
}
