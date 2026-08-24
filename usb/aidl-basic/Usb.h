/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/usb/BnUsb.h>

#include <mutex>

namespace aidl {
namespace android {
namespace hardware {
namespace usb {

struct Usb : public BnUsb {
    Usb();

    ::ndk::ScopedAStatus enableContaminantPresenceDetection(const std::string& in_portName,
                                                            bool in_enable,
                                                            int64_t in_transactionId) override;
    ::ndk::ScopedAStatus enableUsbData(const std::string& in_portName, bool in_enable,
                                       int64_t in_transactionId) override;
    ::ndk::ScopedAStatus enableUsbDataWhileDocked(const std::string& in_portName,
                                                  int64_t in_transactionId) override;
    ::ndk::ScopedAStatus limitPowerTransfer(const std::string& in_portName, bool in_limit,
                                            int64_t in_transactionId) override;
    ::ndk::ScopedAStatus queryPortStatus(int64_t in_transactionId) override;
    ::ndk::ScopedAStatus queryStaticPortInformation(const std::string& in_portName,
                                                    int64_t in_transactionId) override;
    ::ndk::ScopedAStatus resetUsbPort(const std::string& in_portName,
                                      int64_t in_transactionId) override;
    ::ndk::ScopedAStatus setCallback(const std::shared_ptr<IUsbCallback>& in_callback) override;
    ::ndk::ScopedAStatus switchRole(const std::string& in_portName, const PortRole& in_role,
                                    int64_t in_transactionId) override;

  private:
    void queryPortStatusHelper();

    bool mUsbDataEnabled = true;
    std::shared_ptr<IUsbCallback> mCallback;
    std::mutex mMutex;
};

}  // namespace usb
}  // namespace hardware
}  // namespace android
}  // namespace aidl
