/*
 * SPDX-FileCopyrightText: 2016 The CyanogenMod Project
 * SPDX-FileCopyrightText: 2017-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "livedisplay/sdm/Utils.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {
namespace utils {

status_t CheckFeatureVersion(const std::shared_ptr<SDMController>& controller,
                             FeatureVerSw feature) {
    SdmFeatureVersion version{};
    status_t status = controller->getFeatureVersion(feature, &version);
    if (status != android::OK) {
        return status;
    }

    if (version.x <= 0 && version.y <= 0 && version.z <= 0) {
        return android::BAD_VALUE;
    }

    return android::OK;
}

}  // namespace utils
}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
