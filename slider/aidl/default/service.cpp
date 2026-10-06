/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Slider.h"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <linux/input.h>
#include <unistd.h>

#include <optional>

using ::aidl::vendor::lineage::slider::Edge;
using ::aidl::vendor::lineage::slider::Slider;
using ::aidl::vendor::lineage::slider::SliderLocation;

namespace {
constexpr int kWaitAttempts = 60;
constexpr int kWaitLogInterval = 30;

std::optional<Edge> edgeFrom(const std::string& name) {
    if (name == "left") {
        return Edge::LEFT;
    }
    if (name == "top") {
        return Edge::TOP;
    }
    if (name == "right") {
        return Edge::RIGHT;
    }
    if (name == "bottom") {
        return Edge::BOTTOM;
    }
    if (!name.empty()) {
        LOG(ERROR) << "ro.vendor.slider.edge is " << name << ", not an edge";
    }
    return std::nullopt;
}

// Saying nothing about the edge or the offset, or half of it, leaves the
// indicator to place itself.
std::optional<SliderLocation> locationFrom() {
    std::optional<Edge> edge = edgeFrom(::android::base::GetProperty("ro.vendor.slider.edge", ""));
    if (!edge.has_value()) {
        return std::nullopt;
    }

    int offset = ::android::base::GetIntProperty("ro.vendor.slider.offset_pixels", -1);
    if (offset < 0) {
        LOG(ERROR) << "ro.vendor.slider.offset_pixels must be an offset along that edge";
        return std::nullopt;
    }

    SliderLocation location;
    location.edge = *edge;
    location.offsetPixels = offset;
    location.display = ::android::base::GetProperty("ro.vendor.slider.display", "");
    return location;
}
}  // namespace

int main() {
    std::vector<std::string> inputNames;
    for (const std::string& name : ::android::base::Split(
                 ::android::base::GetProperty("ro.vendor.slider.input_name", ""), ",")) {
        std::string trimmed = ::android::base::Trim(name);
        if (!trimmed.empty()) {
            inputNames.push_back(trimmed);
        }
    }
    int absCode = ::android::base::GetIntProperty("ro.vendor.slider.code", -1);

    if (inputNames.empty()) {
        LOG(ERROR) << "ro.vendor.slider.input_name must name an input device";
        return EXIT_FAILURE;
    }

    if (absCode < 0 || absCode > ABS_MAX) {
        LOG(ERROR) << "ro.vendor.slider.code must name an absolute axis";
        return EXIT_FAILURE;
    }

    std::optional<SliderLocation> location = locationFrom();

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<Slider> slider = ndk::SharedRefBase::make<Slider>(absCode, location);

    // The manifest declares the instance whether or not the input device is
    // there yet, so the wait is bounded and says why it is waiting.
    for (int attempt = 0; !slider->start(inputNames, attempt % kWaitLogInterval == 0); attempt++) {
        if (attempt >= kWaitAttempts) {
            LOG(ERROR) << "No slider input device after " << kWaitAttempts << " attempts";
            return EXIT_FAILURE;
        }
        sleep(1);
    }

    const std::string instance = std::string(Slider::descriptor) + "/default";
    binder_status_t status =
            AServiceManager_addService(slider->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
