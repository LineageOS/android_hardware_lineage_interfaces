/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <devices/LedDevice.h>

#define LOG_TAG "LedDevice"

#include <Utils.h>

#include <android-base/logging.h>
#include <fstream>
#include <thread>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

static const uint32_t kDefaultMaxBrightness = 255;

static const std::string kBaseLedsPath = "/sys/class/leds/";

static const std::string kBrightnessNode = "brightness";
static const std::string kMaxBrightnessNode = "max_brightness";

static const std::string kBreathNodes[] = {
        "breath",
        "blink",
};

static const std::string kBlinkNode = "blink";
static const std::string kStartIdxNode = "start_idx";
static const std::string kDutyPctsNode = "duty_pcts";
static const std::string kPauseLoNode = "pause_lo";
static const std::string kPauseHiNode = "pause_hi";
static const std::string kRampStepMsNode = "ramp_step_ms";

static const std::string kTriggerNode = "trigger";
static const std::string kDelayOffNode = "delay_off";
static const std::string kDelayOnNode = "delay_on";

static constexpr int kRampSteps = 8;
static constexpr int kRampMaxStepDurationMs = 50;

LedDevice::LedDevice(std::string name)
    : mName(name), mIdx(0), mBasePath(kBaseLedsPath + name + "/") {
    if (!readFromFile(mBasePath + kMaxBrightnessNode, mMaxBrightness)) {
        mMaxBrightness = kDefaultMaxBrightness;
    }

    for (const auto& node : kBreathNodes) {
        if (std::ifstream(mBasePath + node).good()) {
            mBreathNode = node;
            break;
        }
    }

    mSupportsTimed = std::ifstream(mBasePath + kBlinkNode).good() &&
                     std::ifstream(mBasePath + kStartIdxNode).good() &&
                     std::ifstream(mBasePath + kDutyPctsNode).good() &&
                     std::ifstream(mBasePath + kPauseLoNode).good() &&
                     std::ifstream(mBasePath + kPauseHiNode).good() &&
                     std::ifstream(mBasePath + kRampStepMsNode).good();
}

bool LedDevice::isOk() const {
    return std::ifstream(mBasePath + kBrightnessNode).good();
}

bool LedDevice::setState(const State& state) {
    LightMode mode = LightMode::STATIC;
    switch (state.effect.type) {
        case Effect::Type::FIXED:
            mode = LightMode::STATIC;
            break;
        case Effect::Type::HARDWARE:
            mode = LightMode::BREATH;
            break;
        case Effect::Type::TIMED:
            mode = LightMode::TIMED;
            break;
    }

    uint32_t flashOnMs = 0, flashOffMs = 0;
    if (state.effect.type == Effect::Type::TIMED) {
        flashOnMs = state.effect.timed.onMs;
        flashOffMs = state.effect.timed.offMs;
    }

    switch (mode) {
        case LightMode::TIMED:
            if (supportsMode(mode)) {
                break;
            }
            mode = LightMode::BREATH;
            FALLTHROUGH_INTENDED;
        case LightMode::BREATH:
            if (supportsMode(mode)) {
                break;
            }
            mode = LightMode::STATIC;
            FALLTHROUGH_INTENDED;
        case LightMode::STATIC:
            break;
    }

    return setBrightness(state.color.toBrightness(), mode, flashOnMs, flashOffMs);
}

std::string LedDevice::getName() const {
    return mName;
}

bool LedDevice::supportsMode(LightMode mode) const {
    switch (mode) {
        case LightMode::STATIC:
            return true;
        case LightMode::BREATH:
            return !mBreathNode.empty();
        case LightMode::TIMED:
            return mSupportsTimed;
    }

    return false;
}

static std::string getScaledDutyPercent(uint8_t brightness) {
    std::string output;
    for (int i = 0; i < kRampSteps; i++) {
        if (i != 0) {
            output += ",";
        }
        output += std::to_string(i * 100 * brightness / (0xFF * kRampSteps));
    }
    return output;
}

bool LedDevice::setBrightness(uint8_t value, LightMode mode, uint32_t flashOnMs,
                              uint32_t flashOffMs) {
    // Disable current blinking
    if (mSupportsTimed) {
        writeToFile(mBasePath + kBlinkNode, 0);
    } else {
        writeToFile(mBasePath + kTriggerNode, "none");
    }
    if (supportsMode(LightMode::BREATH)) {
        writeToFile(mBasePath + mBreathNode, 0);
    }

    switch (mode) {
        case LightMode::TIMED: {
            if (mSupportsTimed) {
                int32_t stepDuration = kRampMaxStepDurationMs;
                int32_t pauseLo = flashOffMs;
                int32_t pauseHi = flashOnMs - (stepDuration * kRampSteps * 2);

                if (pauseHi < 0) {
                    stepDuration = flashOnMs / (kRampSteps * 2);
                    pauseHi = 0;
                }

                return writeToFile(mBasePath + kStartIdxNode, mIdx * kRampSteps) &&
                       writeToFile(mBasePath + kDutyPctsNode, getScaledDutyPercent(value)) &&
                       writeToFile(mBasePath + kPauseLoNode, pauseLo) &&
                       writeToFile(mBasePath + kPauseHiNode, pauseHi) &&
                       writeToFile(mBasePath + kRampStepMsNode, stepDuration) &&
                       writeToFile(mBasePath + kBlinkNode, 1);
            } else {
                bool ok = writeToFile(mBasePath + kTriggerNode, "timer");
                if (ok) {
                    using namespace std::chrono_literals;
                    auto retries = 20;
                    while (retries--) {
                        std::this_thread::sleep_for(2ms);

                        ok = writeToFile(mBasePath + kDelayOffNode, flashOffMs);
                        if (!ok) continue;

                        ok = writeToFile(mBasePath + kDelayOnNode, flashOnMs);
                        if (ok) break;
                    }
                }
                return ok;
            }
            break;
        }
        case LightMode::BREATH: {
            return writeToFile(mBasePath + mBreathNode, value > 0 ? 1 : 0);
            break;
        }
        case LightMode::STATIC: {
            return writeToFile(mBasePath + kBrightnessNode, scaleBrightness(value, mMaxBrightness));
            break;
        }
        default: {
            LOG(ERROR) << "Unknown mode: " << mode;
            return false;
            break;
        }
    }
}

void LedDevice::setIdx(int idx) {
    mIdx = idx;
}

void LedDevice::dump(int fd) const {
    dprintf(fd, "Name: %s", mName.c_str());
    dprintf(fd, ", index: %d", mIdx);
    dprintf(fd, ", is ok: %d", isOk());
    dprintf(fd, ", base path: %s", mBasePath.c_str());
    dprintf(fd, ", max brightness: %u", mMaxBrightness);
    dprintf(fd, ", supports breath: %d", supportsMode(LedDevice::LightMode::BREATH));
    dprintf(fd, ", supports timed: %d", supportsMode(LedDevice::LightMode::TIMED));
    dprintf(fd, ", breath node: %s", mBreathNode.c_str());
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
