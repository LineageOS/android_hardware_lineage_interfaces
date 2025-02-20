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

#include <aidl/android/frameworks/stats/IStats.h>
#include <hardware/google/pixel/pixelstats/pixelatoms.pb.h>

#include "SessionMetrics.h"

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using aidl::android::frameworks::stats::IStats;
using android::frameworks::stats::VendorAtomValue;

class MetricUploader {
  public:
    ~MetricUploader() = default;
    MetricUploader(MetricUploader const &) = delete;
    MetricUploader(MetricUploader &&) = delete;
    MetricUploader &operator=(MetricUploader const &) = delete;
    MetricUploader &operator=(MetricUploader &&) = delete;

    bool init();
    bool uploadMetrics(const SessionJankStatsWithThermal &sessMetrics);

    // Singleton
    static MetricUploader *getInstance() {
        static MetricUploader instance{};
        return &instance;
    }

  private:
    MetricUploader() = default;
    bool reportAtom(const int32_t &atomId, std::vector<VendorAtomValue> &&values);
    bool connectIStatsService();

    std::shared_ptr<IStats> mIStatsClient;
};

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
