/*
 * Copyright (C) 2024 The LineageOS Project
 *               2024 Paranoid Android
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"

#include <android-base/properties.h>
#include <fingerprint.sysprop.h>
#include <util/Util.h>

#include <android-base/logging.h>
#include <android-base/strings.h>

namespace aidl::android::hardware::biometrics::fingerprint {

namespace {
constexpr int MAX_ENROLLMENTS_PER_USER = 5;
constexpr char HW_COMPONENT_ID[] = "fingerprintSensor";
constexpr char HW_VERSION[] = "vendor/model/revision";
constexpr char FW_VERSION[] = "1.01";
constexpr char SERIAL_NUMBER[] = "00000001";
constexpr char SW_COMPONENT_ID[] = "matchingAlgorithm";
constexpr char SW_VERSION[] = "vendor/version/revision";
}  // namespace

Fingerprint::Fingerprint(std::shared_ptr<FingerprintConfig> config)
    : mConfig(std::move(config)) {

    std::string backendTypeProp = mConfig->get<std::string>("backend");
    if (backendTypeProp == "hardware") {
        mEngine = std::make_unique<FingerprintEngineHw>(config);
    } else if (backendTypeProp == "hidl") {
        mEngine = std::make_unique<FingerprintEngineHidl>(config);
    } else {
        mEngine = std::make_unique<FingerprintEngine>(config);
    }
    ALOGI("backendTypeProp: %s", backendTypeProp.c_str());

    std::string sensorTypeProp = mConfig->get<std::string>("type");
    if (sensorTypeProp == "udfps" || sensorTypeProp == "udfps_optical") {
        if (sensorTypeProp == "udfps") {
            mSensorType = FingerprintSensorType::UNDER_DISPLAY_ULTRASONIC;
        } else {
            mSensorType = FingerprintSensorType::UNDER_DISPLAY_OPTICAL;
        }
    } else if (sensorTypeProp == "side") {
        mSensorType = FingerprintSensorType::POWER_BUTTON;
    } else if (sensorTypeProp == "home") {
        mSensorType = FingerprintSensorType::HOME_BUTTON;
    } else if (sensorTypeProp == "rear") {
        mSensorType = FingerprintSensorType::REAR;
    } else {
        mSensorType = FingerprintSensorType::UNKNOWN;
        UNIMPLEMENTED(FATAL) << "unrecognized or unimplemented fingerprint behavior: "
                             << sensorTypeProp;
    }
    ALOGI("sensorTypeProp: %s", sensorTypeProp.c_str());
}

std::vector<SensorLocation> Fingerprint::getSensorLocations() {
    std::vector<SensorLocation> locations;

    auto loc = mConfig->get<std::string>("sensor_location");
    auto entries = ::android::base::Split(loc, ",");

    for (const auto& entry : entries) {
        auto isValidStr = false;
        auto dim = ::android::base::Split(entry, "|");

        if (dim.size() != 3 and dim.size() != 4) {
            if (!loc.empty()) {
                ALOGE("Invalid sensor location input (x|y|radius) or (x|y|radius|display): %s",
                      loc.c_str());
            }
        } else {
            int32_t x, y, r;
            std::string d;
            isValidStr = ParseInt(dim[0], &x) && ParseInt(dim[1], &y) && ParseInt(dim[2], &r);
            if (dim.size() == 4) {
                d = dim[3];
                isValidStr = isValidStr && !d.empty();
            }
            if (isValidStr)
                locations.push_back({.sensorLocationX = x,
                                     .sensorLocationY = y,
                                     .sensorRadius = r,
                                     .display = d});
        }
    }

    return locations;
}

ndk::ScopedAStatus Fingerprint::getSensorProps(std::vector<SensorProps>* out) {
    std::vector<common::ComponentInfo> componentInfo = {
            {HW_COMPONENT_ID, HW_VERSION, FW_VERSION, SERIAL_NUMBER, "" /* softwareVersion */},
            {SW_COMPONENT_ID, "" /* hardwareVersion */, "" /* firmwareVersion */,
             "" /* serialNumber */, SW_VERSION}};
    auto sensorId = mConfig->get<std::int32_t>("sensor_id");
    auto sensorStrength = mConfig->get<std::int32_t>("sensor_strength");
    auto navigationGuesture = mConfig->get<bool>("navigation_gesture");
    auto detectInteraction = mConfig->get<bool>("detect_interaction");
    auto displayTouch = mConfig->get<bool>("display_touch");
    auto controlIllumination = mConfig->get<bool>("control_illumination");

    common::CommonProps commonProps = {sensorId, (common::SensorStrength)sensorStrength,
                                       MAX_ENROLLMENTS_PER_USER, componentInfo};

    std::vector<SensorLocation> sensorLocations = getSensorLocations();

    std::vector<std::string> sensorLocationStrings;
    std::transform(sensorLocations.begin(), sensorLocations.end(),
                   std::back_inserter(sensorLocationStrings),
                   [](const SensorLocation& obj) { return obj.toString(); });

    ALOGI("sensor id: %d, type: %s, location: %s", sensorId,
          ::android::internal::ToString(mSensorType).c_str(),
          ::android::base::Join(sensorLocationStrings, ", ").c_str());

    *out = {{commonProps, mSensorType, sensorLocations, navigationGuesture, detectInteraction,
             displayTouch, controlIllumination, std::nullopt}};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Fingerprint::createSession(int32_t /*sensorId*/, int32_t userId,
                                              const std::shared_ptr<ISessionCallback>& cb,
                                              std::shared_ptr<ISession>* out) {
    CHECK(mSession == nullptr || mSession->isClosed()) << "Open session already exists!";

    mSession = SharedRefBase::make<Session>(mEngine, userId, cb);
    *out = mSession;

    mSession->linkToDeath(cb->asBinder().get());

    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
