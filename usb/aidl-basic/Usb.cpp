/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.usb-service.basic"

#include <android-base/file.h>
#include <android-base/logging.h>

#include "Usb.h"

namespace aidl {
namespace android {
namespace hardware {
namespace usb {

Usb::Usb() {}

::ndk::ScopedAStatus Usb::switchRole(const std::string& /*in_portName*/,
                                     const PortRole& /*in_role*/, int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        PortRole role;
        role.set<PortRole::powerRole>(PortPowerRole::NONE);
        auto ret = mCallback->notifyRoleSwitchStatus("", role, Status::NOT_SUPPORTED,
                                                     in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyRoleSwitchStatus error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

void Usb::queryPortStatusHelper() {
    PortStatus portStatus;
    portStatus.portName = "otg_default";
    portStatus.currentDataRole = PortDataRole::DEVICE;
    portStatus.currentPowerRole = PortPowerRole::SINK;
    portStatus.currentMode = PortMode::UFP;
    portStatus.canChangeMode = false;
    portStatus.canChangeDataRole = false;
    portStatus.canChangePowerRole = false;
    portStatus.supportedModes = {PortMode::UFP};
    portStatus.supportedContaminantProtectionModes = {ContaminantProtectionMode::NONE};
    portStatus.supportsEnableContaminantPresenceProtection = false;
    portStatus.contaminantProtectionStatus = ContaminantProtectionStatus::NONE;
    portStatus.supportsEnableContaminantPresenceDetection = false;
    portStatus.contaminantDetectionStatus = ContaminantDetectionStatus::NOT_SUPPORTED;
    portStatus.usbDataStatus = {mUsbDataEnabled ? UsbDataStatus::ENABLED
                                                : UsbDataStatus::DISABLED_FORCE};
    portStatus.powerTransferLimited = false;

    std::vector<PortStatus> portStatuses = {portStatus};

    if (mCallback != nullptr) {
        auto ret = mCallback->notifyPortStatusChange(portStatuses, Status::SUCCESS);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyPortStatusChange error: " << ret.getDescription();
        }
    } else {
        LOG(INFO) << "Notifying userspace skipped. Callback is NULL";
    }
}

::ndk::ScopedAStatus Usb::queryPortStatus(int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    queryPortStatusHelper();

    if (mCallback != nullptr) {
        auto ret = mCallback->notifyQueryPortStatus("all", Status::SUCCESS, in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyQueryPortStatus error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::setCallback(const std::shared_ptr<IUsbCallback>& in_callback) {
    std::lock_guard<std::mutex> lock(mMutex);
    mCallback = in_callback;
    LOG(INFO) << "registering callback";
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::enableContaminantPresenceDetection(const std::string& /*in_portName*/,
                                                             bool /*in_enable*/,
                                                             int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        auto ret = mCallback->notifyContaminantEnabledStatus("", false, Status::NOT_SUPPORTED,
                                                             in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyContaminantEnabledStatus error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::enableUsbData(const std::string& in_portName, bool in_enable,
                                        int64_t in_transactionId) {
    bool success = ::android::base::WriteStringToFile(
            in_enable ? USB_CONTROL_DISABLE : USB_CONTROL_ENABLE, USB_CONTROL_PATH);

    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        auto ret = mCallback->notifyEnableUsbDataStatus(in_portName, in_enable,
                                                        success ? Status::SUCCESS : Status::ERROR,
                                                        in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyEnableUsbDataStatus error: " << ret.getDescription();
        }
    }

    if (success) {
        mUsbDataEnabled = in_enable;
        queryPortStatusHelper();
    } else {
        LOG(ERROR) << __func__ << ": Failed to " << (in_enable ? "enable" : "disable");
    }

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::enableUsbDataWhileDocked(const std::string& /*in_portName*/,
                                                   int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        auto ret = mCallback->notifyEnableUsbDataWhileDockedStatus("", Status::NOT_SUPPORTED,
                                                                   in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyEnableUsbDataWhileDockedStatus error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::limitPowerTransfer(const std::string& /*in_portName*/, bool /*in_limit*/,
                                             int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        auto ret = mCallback->notifyLimitPowerTransferStatus("", false, Status::NOT_SUPPORTED,
                                                             in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyLimitPowerTransferStatus error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::resetUsbPort(const std::string& /*in_portName*/,
                                       int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        auto ret = mCallback->notifyResetUsbPortStatus("", Status::NOT_SUPPORTED, in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyResetUsbPortStatus error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Usb::queryStaticPortInformation(const std::string& /*in_portName*/,
                                                     int64_t in_transactionId) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mCallback != nullptr) {
        StaticPortInformation staticPortInfo;
        auto ret = mCallback->notifyQueryStaticPortInformation(
                "", staticPortInfo, Status::NOT_SUPPORTED, in_transactionId);
        if (!ret.isOk()) {
            LOG(ERROR) << "notifyQueryStaticPortInformation error: " << ret.getDescription();
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

}  // namespace usb
}  // namespace hardware
}  // namespace android
}  // namespace aidl
