/*
 * Copyright (C) 2024 The LineageOS Project
 *               2024 Paranoid Android
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/fingerprint/BnFingerprint.h>

#include "FingerprintConfig.h"
#include "FingerprintEngine.h"
#include "FingerprintEngineHidl.h"
#include "FingerprintEngineHw.h"
#include "Session.h"

using ::aidl::android::hardware::biometrics::fingerprint::FingerprintSensorType;
using ::aidl::android::hardware::biometrics::fingerprint::ISession;
using ::aidl::android::hardware::biometrics::fingerprint::ISessionCallback;
using ::aidl::android::hardware::biometrics::fingerprint::SensorProps;

namespace aidl::android::hardware::biometrics::fingerprint {

class Fingerprint : public BnFingerprint {
  public:
    Fingerprint(std::shared_ptr<FingerprintConfig> config);

    ndk::ScopedAStatus getSensorProps(std::vector<SensorProps>* _aidl_return) override;
    ndk::ScopedAStatus createSession(int32_t sensorId, int32_t userId,
                                     const std::shared_ptr<ISessionCallback>& cb,
                                     std::shared_ptr<ISession>* out) override;

  private:
    std::vector<SensorLocation> getSensorLocations();

    std::shared_ptr<FingerprintConfig> mConfig;
    std::shared_ptr<FingerprintEngine> mEngine;
    std::shared_ptr<Session> mSession;
    FingerprintSensorType mSensorType;
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
