/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <devices/RgbLedDevice.h>

#define LOG_TAG "RgbLedDevice"

#include <Utils.h>

#include <android-base/logging.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

RgbLedDevice::RgbLedDevice(LedDevice red, LedDevice green, LedDevice blue, std::string rgbSyncNode)
    : mRed(red), mGreen(green), mBlue(blue), mRgbSyncNode(rgbSyncNode), mRoles(Role::NONE) {
    if (mRed.isOk()) {
        mRoles |= Role::RED;
    }
    if (mGreen.isOk()) {
        mRoles |= Role::GREEN;
    }
    if (mBlue.isOk()) {
        mRoles |= Role::BLUE;
    }
    mRed.setIdx(0);
    mGreen.setIdx(1);
    mBlue.setIdx(2);
}

bool RgbLedDevice::isOk() const {
    return mRoles != Role::NONE;
}

bool RgbLedDevice::setState(const State& state) {
    bool rc = true;

    if (mRoles == Role::NONE) {
        LOG(ERROR) << "No LEDs found";
        return false;
    }

    LedDevice::LightMode mode = LedDevice::LightMode::STATIC;
    switch (state.effect.type) {
        case Effect::Type::FIXED:
            mode = LedDevice::LightMode::STATIC;
            break;
        case Effect::Type::HARDWARE:
            mode = LedDevice::LightMode::BREATH;
            break;
        case Effect::Type::TIMED:
            mode = LedDevice::LightMode::TIMED_QCOM;
            break;
    }

    uint32_t flashOnMs = 0, flashOffMs = 0;
    if (state.effect.type == Effect::Type::TIMED) {
        flashOnMs = state.effect.timed.onMs;
        flashOffMs = state.effect.timed.offMs;
    }

    switch (mode) {
        case LedDevice::LightMode::TIMED_QCOM:
            if (supportsMode(mode)) {
                break;
            }
            mode = LedDevice::LightMode::TIMED_UPSTREAM;
            FALLTHROUGH_INTENDED;
        case LedDevice::LightMode::TIMED_UPSTREAM:
            if (supportsMode(mode)) {
                break;
            }
            mode = LedDevice::LightMode::BREATH;
            FALLTHROUGH_INTENDED;
        case LedDevice::LightMode::BREATH:
            if (supportsMode(mode)) {
                break;
            }
            mode = LedDevice::LightMode::STATIC;
            FALLTHROUGH_INTENDED;
        case LedDevice::LightMode::STATIC:
            break;
    }

    if (mode == LedDevice::LightMode::TIMED_QCOM && supportsRgbSync()) {
        rc &= writeToFile(mRgbSyncNode, 0);
    }

    if (mRoles == Role::ALL) {
        rc &= mRed.setBrightness(state.color.red, mode, flashOnMs, flashOffMs);
        rc &= mGreen.setBrightness(state.color.green, mode, flashOnMs, flashOffMs);
        rc &= mBlue.setBrightness(state.color.blue, mode, flashOnMs, flashOffMs);
    } else {
        // Check if we have only one LED
        if (mRoles == Role::RED) {
            rc &= mRed.setBrightness(state.color.toBrightness(), mode, flashOnMs, flashOffMs);
        } else if (mRoles == Role::GREEN) {
            rc &= mGreen.setBrightness(state.color.toBrightness(), mode, flashOnMs, flashOffMs);
        } else if (mRoles == Role::BLUE) {
            rc &= mBlue.setBrightness(state.color.toBrightness(), mode, flashOnMs, flashOffMs);
        } else {
            // We only have two LEDs, blend the missing color in the other two
            if ((mRoles & Role::RED) == Role::NONE) {
                rc &= mBlue.setBrightness((state.color.blue + state.color.red) / 2, mode, flashOnMs,
                                          flashOffMs);
                rc &= mGreen.setBrightness((state.color.green + state.color.red) / 2, mode,
                                           flashOnMs, flashOffMs);
            } else if ((mRoles & Role::GREEN) == Role::NONE) {
                rc &= mRed.setBrightness((state.color.red + state.color.green) / 2, mode, flashOnMs,
                                         flashOffMs);
                rc &= mBlue.setBrightness((state.color.blue + state.color.green) / 2, mode,
                                          flashOnMs, flashOffMs);
            } else if ((mRoles & Role::BLUE) == Role::NONE) {
                rc &= mRed.setBrightness((state.color.red + state.color.blue) / 2, mode, flashOnMs,
                                         flashOffMs);
                rc &= mGreen.setBrightness((state.color.green + state.color.blue) / 2, mode,
                                           flashOnMs, flashOffMs);
            }
        }
    }

    if (mode == LedDevice::LightMode::TIMED_QCOM && supportsRgbSync()) {
        rc &= writeToFile(mRgbSyncNode, 1);
    }

    return rc;
}

void RgbLedDevice::dump(int fd) const {
    dprintf(fd, "Is ok: %d", isOk());
    dprintf(fd, ", supports breath: %d", supportsMode(LedDevice::LightMode::BREATH));
    dprintf(fd, ", supports timed upstream: %d",
            supportsMode(LedDevice::LightMode::TIMED_UPSTREAM));
    dprintf(fd, ", supports timed qcom: %d", supportsMode(LedDevice::LightMode::TIMED_QCOM));
    dprintf(fd, ", supports RGB sync: %d", supportsRgbSync());
    dprintf(fd, ", roles:");
    if (mRoles != Role::NONE) {
        if (mRoles & Role::RED) {
            dprintf(fd, "\nRed: ");
            mRed.dump(fd);
        }
        if (mRoles & Role::GREEN) {
            dprintf(fd, "\nGreen: ");
            mGreen.dump(fd);
        }
        if (mRoles & Role::BLUE) {
            dprintf(fd, "\nBlue: ");
            mBlue.dump(fd);
        }
    } else {
        dprintf(fd, " None");
    }
}

bool RgbLedDevice::supportsMode(LedDevice::LightMode mode) const {
    return (!mRed.isOk() || mRed.supportsMode(mode)) &&
           (!mGreen.isOk() || mGreen.supportsMode(mode)) &&
           (!mBlue.isOk() || mBlue.supportsMode(mode));
}

bool RgbLedDevice::supportsRgbSync() const {
    return std::ifstream(mRgbSyncNode).good();
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
