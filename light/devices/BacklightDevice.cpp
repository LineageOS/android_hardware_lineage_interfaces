/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <devices/BacklightDevice.h>

#define LOG_TAG "BacklightDevice"

#include <Utils.h>

#include <android-base/logging.h>
#include <fstream>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

static const std::string kBacklightBasePath = "/sys/class/backlight/";
static const uint32_t kDefaultMaxBrightness = 255;

static const std::string kBrightnessNode = "brightness";
static const std::string kMaxBrightnessNode = "max_brightness";

BacklightDevice::BacklightDevice(std::string name)
    : mName(name), mBasePath(kBacklightBasePath + name + "/") {
    if (!readFromFile(mBasePath + kMaxBrightnessNode, mMaxBrightness)) {
        mMaxBrightness = kDefaultMaxBrightness;
    }
};

bool BacklightDevice::isOk() const {
    return std::ifstream(mBasePath + kBrightnessNode).good();
}

bool BacklightDevice::setState(const State& state) {
    return setBrightness(state.color.toBrightness());
}

std::string BacklightDevice::getName() const {
    return mName;
}

bool BacklightDevice::setBrightness(uint8_t value) {
    return writeToFile(mBasePath + kBrightnessNode, scaleBrightness(value, mMaxBrightness));
}

void BacklightDevice::dump(int fd) const {
    dprintf(fd, "Name: %s", mName.c_str());
    dprintf(fd, ", is ok: %d", isOk());
    dprintf(fd, ", base path: %s", mBasePath.c_str());
    dprintf(fd, ", max brightness: %u", mMaxBrightness);
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
