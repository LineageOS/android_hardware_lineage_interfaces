/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Devices.h>

#define LOG_TAG "Devices"

#include <android-base/logging.h>
#include <filesystem>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

namespace {

std::vector<std::string> getSubDirs(const std::string& path) {
    std::vector<std::string> subdirs;
    std::filesystem::path p(path);

    CHECK(std::filesystem::is_directory(p));

    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        if (entry.is_directory()) {
            subdirs.push_back(entry.path().filename().string());
        }
    }

    return subdirs;
}

}  // namespace

static const std::string kBacklightDevices[] = {
        "backlight",
        "panel0-backlight",
        "panel0-backlight-ex",
        "sprd_backlight",
};

static std::vector<BacklightDevice> getBacklightDevices() {
    std::vector<BacklightDevice> devices;

    for (const auto& device : kBacklightDevices) {
        BacklightDevice backlight(device);
        if (backlight.isOk()) {
            LOG(INFO) << "Found backlight device: " << backlight.getName();
            devices.push_back(backlight);
        }
    }

#ifdef SCAN_FOR_BACKLIGHT_DEVICES
    if (devices.empty()) {
        LOG(INFO) << "Scanning for backlight devices.";
        for (const auto& device : getSubDirs("/sys/class/backlight/")) {
            BacklightDevice backlight(device);
            if (backlight.isOk()) {
                LOG(INFO) << "Found backlight device: " << backlight.getName();
                devices.push_back(backlight);
            }
        }
    }
#endif

    return devices;
}

static const std::string kLedBacklightDevices[] = {
        "lcd-backlight",
        "lcd-backlight-ex",
        "lcd_backlight0",
};

static std::vector<LedDevice> getBacklightLedDevices() {
    std::vector<LedDevice> devices;

    for (const auto& device : kLedBacklightDevices) {
        LedDevice backlight(device);
        if (backlight.isOk()) {
            LOG(INFO) << "Found backlight LED device: " << backlight.getName();
            devices.push_back(backlight);
        }
    }

    return devices;
}

static const std::string kButtonLedDevices[] = {
        "button-backlight",
        "button-backlight1",
        "button-backlight2",
};

static std::vector<LedDevice> getButtonLedDevices() {
    std::vector<LedDevice> devices;

    for (const auto& device : kButtonLedDevices) {
        LedDevice button(device);
        if (button.isOk()) {
            LOG(INFO) << "Found button LED device: " << button.getName();
            devices.emplace_back(button);
        }
    }

    return devices;
}

static const std::string kKeyboardLedDevices[] = {
        "keyboard-backlight",
};

static std::vector<LedDevice> getKeyboardLedDevices() {
    std::vector<LedDevice> devices;

    for (const auto& device : kKeyboardLedDevices) {
        LedDevice keyboard(device);
        if (keyboard.isOk()) {
            LOG(INFO) << "Found keyboard LED device: " << keyboard.getName();
            devices.emplace_back(keyboard);
        }
    }

    return devices;
}

static const std::string kRgbLedDevices[][4] = {
        {"red", "green", "blue", "/sys/class/leds/rgb/rgb_blink"},
        {"red:indicator", "green:indicator", "blue:indicator", ""},
        {"red:status", "green:status", "blue:status", ""},
};

static std::vector<RgbLedDevice> getNotificationRgbLedDevices() {
    std::vector<RgbLedDevice> devices;

    for (const auto& device : kRgbLedDevices) {
        LedDevice red(device[0]);
        LedDevice green(device[1]);
        LedDevice blue(device[2]);

        RgbLedDevice rgbLedDevice(red, green, blue, device[3]);
        if (rgbLedDevice.isOk()) {
            LOG(INFO) << "Found notification RGB LED device: " << red.getName() << ", "
                      << green.getName() << ", " << blue.getName();
            devices.emplace_back(red, green, blue, device[3]);
        }
    }

    return devices;
}

static const std::string kNotificationLedDevices[] = {
        "charging", "left", "white", "white:indicator", "white:status",
};

static std::vector<LedDevice> getNotificationLedDevices() {
    std::vector<LedDevice> devices;

    for (const auto& device : kNotificationLedDevices) {
        LedDevice notification(device);
        if (notification.isOk()) {
            LOG(INFO) << "Found notification LED device: " << notification.getName();
            devices.emplace_back(notification);
        }
    }

    return devices;
}

Devices::Devices()
    : mBacklightDevices(getBacklightDevices()),
      mBacklightLedDevices(getBacklightLedDevices()),
      mButtonLedDevices(getButtonLedDevices()),
      mKeyboardLedDevices(getKeyboardLedDevices()),
      mNotificationRgbLedDevices(getNotificationRgbLedDevices()),
      mNotificationLedDevices(getNotificationLedDevices()) {
    if (!hasBacklightDevices()) {
        LOG(INFO) << "No backlight devices found";
    }

    if (!hasButtonDevices()) {
        LOG(INFO) << "No button devices found";
    }

    if (!hasKeyboardDevices()) {
        LOG(INFO) << "No keyboard devices found";
    }

    if (!hasNotificationDevices()) {
        LOG(INFO) << "No notification devices found";
    }
}

bool Devices::hasBacklightDevices() const {
    return !mBacklightDevices.empty() || !mBacklightLedDevices.empty();
}

bool Devices::hasButtonDevices() const {
    return !mButtonLedDevices.empty();
}

bool Devices::hasKeyboardDevices() const {
    return !mKeyboardLedDevices.empty();
}

bool Devices::hasNotificationDevices() const {
    return !mNotificationRgbLedDevices.empty() || !mNotificationLedDevices.empty();
}

void Devices::setBacklightState(const State& state) {
    for (auto& device : mBacklightDevices) {
        device.setState(state);
    }
    for (auto& device : mBacklightLedDevices) {
        device.setState(state);
    }
}

void Devices::setButtonsState(const State& state) {
    for (auto& device : mButtonLedDevices) {
        device.setState(state);
    }
}

void Devices::setKeyboardState(const State& state) {
    for (auto& device : mKeyboardLedDevices) {
        device.setState(state);
    }
}

void Devices::setNotificationState(const State& state) {
    for (auto& device : mNotificationRgbLedDevices) {
        device.setState(state);
    }

    for (auto& device : mNotificationLedDevices) {
        device.setState(state);
    }
}

void Devices::dump(int fd) const {
    dprintf(fd, "Backlight devices:\n");
    for (const auto& device : mBacklightDevices) {
        dprintf(fd, "- ");
        device.dump(fd);
        dprintf(fd, "\n");
    }
    dprintf(fd, "\n");

    dprintf(fd, "Backlight LED devices:\n");
    for (const auto& device : mBacklightLedDevices) {
        dprintf(fd, "- ");
        device.dump(fd);
        dprintf(fd, "\n");
    }
    dprintf(fd, "\n");

    dprintf(fd, "Button LED devices:\n");
    for (const auto& device : mButtonLedDevices) {
        dprintf(fd, "- ");
        device.dump(fd);
        dprintf(fd, "\n");
    }
    dprintf(fd, "\n");

    dprintf(fd, "Keyboard LED devices:\n");
    for (const auto& device : mKeyboardLedDevices) {
        dprintf(fd, "- ");
        device.dump(fd);
        dprintf(fd, "\n");
    }

    dprintf(fd, "Notification RGB LED devices:\n");
    for (const auto& device : mNotificationRgbLedDevices) {
        dprintf(fd, "- ");
        device.dump(fd);
        dprintf(fd, "\n");
    }
    dprintf(fd, "\n");

    dprintf(fd, "Notification LED devices:\n");
    for (const auto& device : mNotificationLedDevices) {
        dprintf(fd, "- ");
        device.dump(fd);
        dprintf(fd, "\n");
    }

    return;
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
