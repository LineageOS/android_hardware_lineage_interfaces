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
    if (mRed.exists()) {
        mRoles |= Role::RED;
    }
    if (mGreen.exists()) {
        mRoles |= Role::GREEN;
    }
    if (mBlue.exists()) {
        mRoles |= Role::BLUE;
    }
    mRed.setIdx(0);
    mGreen.setIdx(1);
    mBlue.setIdx(2);
}

bool RgbLedDevice::exists() const {
    return mRoles != Role::NONE;
}

bool RgbLedDevice::supportsBreath() const {
    return (!mRed.exists() || mRed.supportsBreath()) &&
           (!mGreen.exists() || mGreen.supportsBreath()) &&
           (!mBlue.exists() || mBlue.supportsBreath());
}

bool RgbLedDevice::supportsTimed() const {
    return (!mRed.exists() || mRed.supportsTimed()) &&
           (!mGreen.exists() || mGreen.supportsTimed()) &&
           (!mBlue.exists() || mBlue.supportsTimed());
}

bool RgbLedDevice::supportsRgbSync() const {
    return std::ifstream(mRgbSyncNode).good();
}

bool RgbLedDevice::setBrightness(Color color, LightMode mode, uint32_t flashOnMs,
                                 uint32_t flashOffMs) {
    bool rc = true;

    if (mRoles == Role::NONE) {
        LOG(ERROR) << "No LEDs found";
        return false;
    }

    if (mode == LightMode::TIMED && !supportsTimed()) {
        // Not all LEDs support timed mode, force breathing mode
        mode = LightMode::BREATH;
    }

    if (mode == LightMode::BREATH && !supportsBreath()) {
        // Not all LEDs support breathing, force static mode
        mode = LightMode::STATIC;
    }

    if (mode == LightMode::TIMED && supportsRgbSync()) {
        rc &= writeToFile(mRgbSyncNode, 0);
    }

    if (mRoles == Role::ALL) {
        rc &= mRed.setBrightness(color.red, mode, flashOnMs, flashOffMs);
        rc &= mGreen.setBrightness(color.green, mode, flashOnMs, flashOffMs);
        rc &= mBlue.setBrightness(color.blue, mode, flashOnMs, flashOffMs);
    } else {
        // Check if we have only one LED
        if (mRoles == Role::RED) {
            rc &= mRed.setBrightness(color.toBrightness(), mode, flashOnMs, flashOffMs);
        } else if (mRoles == Role::GREEN) {
            rc &= mGreen.setBrightness(color.toBrightness(), mode, flashOnMs, flashOffMs);
        } else if (mRoles == Role::BLUE) {
            rc &= mBlue.setBrightness(color.toBrightness(), mode, flashOnMs, flashOffMs);
        } else {
            // We only have two LEDs, blend the missing color in the other two
            if ((mRoles & Role::RED) == Role::NONE) {
                rc &= mBlue.setBrightness((color.blue + color.red) / 2, mode, flashOnMs,
                                          flashOffMs);
                rc &= mGreen.setBrightness((color.green + color.red) / 2, mode, flashOnMs,
                                           flashOffMs);
            } else if ((mRoles & Role::GREEN) == Role::NONE) {
                rc &= mRed.setBrightness((color.red + color.green) / 2, mode, flashOnMs,
                                         flashOffMs);
                rc &= mBlue.setBrightness((color.blue + color.green) / 2, mode, flashOnMs,
                                          flashOffMs);
            } else if ((mRoles & Role::BLUE) == Role::NONE) {
                rc &= mRed.setBrightness((color.red + color.blue) / 2, mode, flashOnMs, flashOffMs);
                rc &= mGreen.setBrightness((color.green + color.blue) / 2, mode, flashOnMs,
                                           flashOffMs);
            }
        }
    }

    if (mode == LightMode::TIMED && supportsRgbSync()) {
        rc &= writeToFile(mRgbSyncNode, 1);
    }

    return rc;
}

void RgbLedDevice::dump(int fd) const {
    dprintf(fd, "Exists: %d", exists());
    dprintf(fd, ", supports breath: %d", supportsBreath());
    dprintf(fd, ", supports timed: %d", supportsTimed());
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

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
