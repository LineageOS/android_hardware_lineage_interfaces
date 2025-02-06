/*
 * Copyright 2025 The Android Open Source Project
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

#pragma once
#include <aidl/android/hardware/thermal/BnThermalChangedCallback.h>
#include <aidl/android/hardware/thermal/IThermal.h>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using ::aidl::android::hardware::thermal::BnThermalChangedCallback;
using ::aidl::android::hardware::thermal::IThermal;
using ::aidl::android::hardware::thermal::Temperature;
using ::aidl::android::hardware::thermal::TemperatureType;
using ::aidl::android::hardware::thermal::ThrottlingSeverity;

/**
 * Listen to the device thermal throttling status, so it could be used to
 * aggregate the performance metrics with the thermal states.
 */
class ThermalStateListener {
  public:
    ~ThermalStateListener() = default;
    ThermalStateListener(ThermalStateListener const &) = delete;
    ThermalStateListener(ThermalStateListener &&) = delete;
    ThermalStateListener &operator=(ThermalStateListener const &) = delete;
    ThermalStateListener &operator=(ThermalStateListener &&) = delete;

    bool init();
    ThrottlingSeverity getThermalThrotSev();

    // Singleton
    static ThermalStateListener *getInstance() {
        static ThermalStateListener instance{};
        return &instance;
    }

  private:
    ThermalStateListener() = default;
    bool connectThermalHal();
    bool registerCallback();
    void thermalCallback(const Temperature &temp);

    class ThermalCallback : public BnThermalChangedCallback {
      public:
        ThermalCallback(std::function<void(const Temperature &)> notify_function)
            : mNotifyFunction(notify_function) {}

        ndk::ScopedAStatus notifyThrottling(const Temperature &temperature) override {
            mNotifyFunction(temperature);
            return ndk::ScopedAStatus::ok();
        }

      private:
        std::function<void(const Temperature &)> mNotifyFunction;
    };

    std::shared_ptr<IThermal> mThermalAIDL;
    std::shared_ptr<ThermalCallback> mThermalCallback;
    std::atomic<ThrottlingSeverity> mThermalThrotSev{ThrottlingSeverity::NONE};
};

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
