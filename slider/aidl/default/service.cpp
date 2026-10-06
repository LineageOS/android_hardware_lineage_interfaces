/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Slider.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <linux/input.h>
#include <slider.sysprop.h>
#include <unistd.h>

#include <optional>

using ::aidl::vendor::lineage::slider::Edge;
using ::aidl::vendor::lineage::slider::Slider;
using ::aidl::vendor::lineage::slider::SliderLocation;

namespace properties = ::vendor::lineage::slider::SliderProperties;

namespace {
constexpr int kWaitAttempts = 60;
constexpr int kWaitLogInterval = 30;

Edge edgeOf(properties::edge_values edge) {
    switch (edge) {
        case properties::edge_values::LEFT:
            return Edge::LEFT;
        case properties::edge_values::TOP:
            return Edge::TOP;
        case properties::edge_values::RIGHT:
            return Edge::RIGHT;
        case properties::edge_values::BOTTOM:
            break;
    }
    return Edge::BOTTOM;
}

// Saying nothing about the edge or the offset, or half of it, leaves the
// indicator to place itself.
std::optional<SliderLocation> locationFrom() {
    std::optional<properties::edge_values> edge = properties::edge();
    std::optional<std::int32_t> offset = properties::offset_pixels();
    if (!edge.has_value() || !offset.has_value()) {
        if (edge.has_value() != offset.has_value()) {
            LOG(ERROR) << "a slider edge needs an offset along it, and the other way round";
        }
        return std::nullopt;
    }

    if (*offset < 0) {
        LOG(ERROR) << "ro.vendor.slider.offset_pixels is " << *offset << ", not an offset";
        return std::nullopt;
    }

    SliderLocation location;
    location.edge = edgeOf(*edge);
    location.offsetPixels = *offset;
    location.display = properties::display().value_or("");
    return location;
}
}  // namespace

int main() {
    std::vector<std::string> inputNames;
    for (const std::optional<std::string>& name : properties::input_name()) {
        if (name.has_value() && !name->empty()) {
            inputNames.push_back(*name);
        }
    }
    int absCode = properties::code().value_or(-1);

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
