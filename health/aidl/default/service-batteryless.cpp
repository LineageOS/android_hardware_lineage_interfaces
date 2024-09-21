/*
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#define LOG_TAG "android.hardware.health-service.batteryless"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#include <android/binder_interface_utils.h>
#include <cutils/klog.h>
#include <health-impl/ChargerUtils.h>
#include <health-impl/Health.h>
#include <health/utils.h>

using android::base::GetProperty;
using android::base::StartsWith;

using aidl::android::hardware::health::BatteryHealth;
using aidl::android::hardware::health::BatteryStatus;
using aidl::android::hardware::health::HalHealthLoop;
using aidl::android::hardware::health::Health;

namespace aidl::android::hardware::health {
class HealthImpl : public Health {
  public:
    HealthImpl(std::string_view instance_name, std::unique_ptr<struct healthd_config>&& config)
        : Health(instance_name, std::move(config)) {
        isLineageSystem = StartsWith(GetProperty("ro.build.flavor", ""), "lineage_");
        KLOG_INFO(LOG_TAG, "isLineageSystem = %s", isLineageSystem ? "true" : "false");
    }

  protected:
    void UpdateHealthInfo(HealthInfo* health_info) override {
        health_info->chargerAcOnline = true;
        health_info->chargerUsbOnline = false;
        health_info->chargerWirelessOnline = false;
        health_info->chargerDockOnline = false;
        health_info->batteryStatus = isLineageSystem ? BatteryStatus::UNKNOWN : BatteryStatus::CHARGING;
        health_info->batteryHealth = isLineageSystem ? BatteryHealth::UNKNOWN : BatteryHealth::GOOD;
        health_info->batteryPresent = isLineageSystem ? false : true;
        health_info->batteryLevel = 100;
    }

  private:
    bool isLineageSystem;
};
}  // namespace aidl::android::hardware::health

int main() {
#ifdef __ANDROID_RECOVERY__
    android::base::InitLogging(NULL, android::base::KernelLogger);
#endif
    auto config = std::make_unique<healthd_config>();
    ::android::hardware::health::InitHealthdConfig(config.get());
    auto binder = ndk::SharedRefBase::make<aidl::android::hardware::health::HealthImpl>(
            "default", std::move(config));

    KLOG_INFO(LOG_TAG, "Starting health HAL.");
    auto hal_health_loop = std::make_shared<HalHealthLoop>(binder, binder);
    return hal_health_loop->StartLoop();
}
