/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
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
using ::aidl::vendor::lineage::slider::Direction;
using ::aidl::vendor::lineage::slider::Edge;
using ::aidl::vendor::lineage::slider::Slider;
using ::aidl::vendor::lineage::slider::SliderLocation;
using ::aidl::vendor::lineage::slider::Source;

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

Direction directionOf(properties::direction_values direction) {
    switch (direction) {
        case properties::direction_values::UP:
            return Direction::UP;
        case properties::direction_values::RIGHT:
            return Direction::RIGHT;
        case properties::direction_values::DOWN:
            return Direction::DOWN;
        case properties::direction_values::LEFT:
            break;
    }
    return Direction::LEFT;
}

bool runsAlong(properties::edge_values edge, properties::direction_values direction) {
    const bool verticalEdge =
            edge == properties::edge_values::LEFT || edge == properties::edge_values::RIGHT;
    const bool verticalTravel = direction == properties::direction_values::UP ||
                                direction == properties::direction_values::DOWN;
    return verticalEdge == verticalTravel;
}

// A device that says nothing about where the slider sits, or says only
// part of it, leaves the indicator to place itself.
std::optional<SliderLocation> locationFrom() {
    std::optional<properties::edge_values> edge = properties::edge();
    std::optional<properties::direction_values> direction = properties::direction();
    std::optional<std::int32_t> offset = properties::offset_pixels();
    if (!edge.has_value() && !direction.has_value() && !offset.has_value()) {
        return std::nullopt;
    }

    if (!edge.has_value() || !direction.has_value() || !offset.has_value()) {
        LOG(ERROR) << "a slider sits on an edge, travels along it and has an offset along it, "
                      "so it takes all three or none";
        return std::nullopt;
    }

    if (*offset < 0) {
        LOG(ERROR) << "ro.vendor.slider.offset_pixels is " << *offset << ", not an offset";
        return std::nullopt;
    }

    if (!runsAlong(*edge, *direction)) {
        LOG(ERROR) << "ro.vendor.slider.direction crosses the edge rather than running along it";
        return std::nullopt;
    }

    SliderLocation location;
    location.edge = edgeOf(*edge);
    location.direction = directionOf(*direction);
    location.offsetPixels = *offset;
    location.display = properties::display().value_or("");
    return location;
}

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

    std::optional<SliderLocation> location = locationFrom();

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<Slider> slider = ndk::SharedRefBase::make<Slider>(std::move(source), location);

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
