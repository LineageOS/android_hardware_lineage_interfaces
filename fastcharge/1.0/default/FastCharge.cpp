/*
 * Copyright (C) 2020-2021 The LineageOS Project
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

#define LOG_TAG "fastcharge@1.0-service"

#define FASTCHARGE_DEFAULT_SETTING true

#include "FastCharge.h"
#include <android-base/logging.h>
#include <cutils/properties.h>

#include <fstream>
#include <iostream>

namespace vendor {
namespace lineage {
namespace fastcharge {
namespace V1_0 {
namespace implementation {

static constexpr const char* kFastChargingProp = "persist.vendor.lineage.fastchg_enabled";

static const std::string kFastChargingPaths[] = {
    "/sys/class/asuslib/slow_charging_enable",
    "/sys/class/qcom-battery/restricted_charging",
    "/sys/class/sec/switch/afc_disable",
};

/*
 * Write value to path and close file.
 */
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);

    if (!file) {
        PLOG(ERROR) << "Failed to open: " << path;
        return;
    }

    LOG(DEBUG) << "write: " << path << " value: " << value;

    file << value << std::endl;

    if (!file) {
        PLOG(ERROR) << "Failed to write: " << path << " value: " << value;
    }
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);

    if (!file) {
        PLOG(ERROR) << "Failed to open: " << path;
        return def;
    }

    T result;

    file >> result;

    if (file.fail()) {
        PLOG(ERROR) << "Failed to read: " << path;
        return def;
    } else {
        LOG(DEBUG) << "read: " << path << " value: " << result;
        return result;
    }
}

FastCharge::FastCharge() {
    for (const auto& path : kFastChargingPaths) {
        if (std::ifstream(path).good()) {
            mPath = path;
        }
    }

    setEnabled(property_get_bool(kFastChargingProp, FASTCHARGE_DEFAULT_SETTING));
}

Return<bool> FastCharge::isEnabled() {
    return get(mPath, 0) < 1;
}

Return<bool> FastCharge::setEnabled(bool enable) {
    set(mPath, enable ? 0 : 1);

    bool enabled = isEnabled();
    property_set(kFastChargingProp, enabled ? "true" : "false");

    return enabled;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace fastcharge
}  // namespace lineage
}  // namespace vendor
