/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <linux/input.h>
#include <slider.sysprop.h>
#include <unistd.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "AxisSource.h"
#include "Slider.h"
#include "Source.h"

using ::aidl::vendor::lineage::slider::AxisSource;
using ::aidl::vendor::lineage::slider::Slider;
using ::aidl::vendor::lineage::slider::Source;

namespace properties = ::vendor::lineage::slider::SliderProperties;

namespace {
constexpr int kWaitAttempts = 60;
constexpr int kWaitLogInterval = 30;

std::unique_ptr<Source> axisSource(const std::vector<std::string>& inputNames) {
    int absCode = properties::code().value_or(-1);
    if (absCode < 0 || absCode > ABS_MAX) {
        LOG(ERROR) << "ro.vendor.slider.code must name an absolute axis";
        return nullptr;
    }

    return std::make_unique<AxisSource>(inputNames, absCode);
}
}  // namespace

int main() {
    std::vector<std::string> inputNames;
    for (const std::optional<std::string>& name : properties::input_name()) {
        if (name.has_value() && !name->empty()) {
            inputNames.push_back(*name);
        }
    }

    if (inputNames.empty()) {
        LOG(ERROR) << "ro.vendor.slider.input_name must name an input device";
        return EXIT_FAILURE;
    }

    std::unique_ptr<Source> source = axisSource(inputNames);
    if (source == nullptr) {
        return EXIT_FAILURE;
    }

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<Slider> slider = ndk::SharedRefBase::make<Slider>(std::move(source));

    // The manifest declares the instance whether or not the hardware is there
    // yet, so the wait is bounded and says why it is waiting.
    for (int attempt = 0; !slider->start(attempt % kWaitLogInterval == 0); attempt++) {
        if (attempt >= kWaitAttempts) {
            LOG(ERROR) << "No slider input device after " << kWaitAttempts << " attempts";
            return EXIT_FAILURE;
        }
        sleep(1);
    }

    const std::string instance = std::string(Slider::descriptor) + "/default";
    binder_status_t status = AServiceManager_addService(slider->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
