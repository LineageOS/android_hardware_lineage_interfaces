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

#include "MetricUploader.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using android::frameworks::stats::VendorAtom;

bool MetricUploader::connectIStatsService() {
    if (mIStatsClient) {
        LOG(INFO) << "IStats service client exists, skip this";
        return true;
    }

    const std::string iStatsServiceName = std::string(IStats::descriptor) + "/default";

    if (!AServiceManager_isDeclared(iStatsServiceName.c_str())) {
        LOG(ERROR) << "IStats service not declared";
        return false;
    }

    ndk::SpAIBinder iStatsBinder =
            ndk::SpAIBinder(AServiceManager_waitForService(iStatsServiceName.c_str()));
    if (iStatsBinder.get() == nullptr) {
        LOG(ERROR) << "Cannot get IStats binder!";
        return false;
    }

    mIStatsClient = IStats::fromBinder(iStatsBinder);
    if (mIStatsClient == nullptr) {
        LOG(ERROR) << "Cannot create IStats AIDL client!";
        return false;
    }

    LOG(INFO) << "Connected to IStats service.";
    return true;
}

bool MetricUploader::init() {
    return connectIStatsService();
}

bool MetricUploader::reportAtom(const int32_t &atomId, std::vector<VendorAtomValue> &&values) {
    LOG(VERBOSE) << "Reporting powerhal metrics ...";
    VendorAtom event = {.reverseDomainName = "", .atomId = atomId, .values = std::move(values)};
    if (!mIStatsClient) {
        if (!connectIStatsService()) {
            LOG(ERROR) << "Don't have valid connection to IStats service!";
            return false;
        }
    }
    const ndk::ScopedAStatus ret = mIStatsClient->reportVendorAtom(event);
    if (!ret.isOk()) {
        LOG(ERROR) << "Failed at reporting atom: " << ret.getMessage();
        return false;
    }
    return true;
}

bool MetricUploader::uploadMetrics(const SessionJankStatsWithThermal &sessMetrics) {
    // TODO(guibing): Store the sessMetrics into the format of the metric atom
    // and then call "reportAtom" to upload them.
    std::string sessMetricDescriptor = std::string(toString(sessMetrics.scenarioType)) + "-" +
                                       toString(sessMetrics.frameTimelineType);
    if (sessMetrics.uid) {
        sessMetricDescriptor += "-" + std::to_string(sessMetrics.uid.value());
    }
    LOG(VERBOSE) << "Uploading session metrics for " << sessMetricDescriptor;
    return true;
}

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
