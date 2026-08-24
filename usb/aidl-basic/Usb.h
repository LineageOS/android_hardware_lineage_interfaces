/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

    std::shared_ptr<IUsbCallback> mCallback;
    std::mutex mMutex;
};

}  // namespace usb
}  // namespace hardware
}  // namespace android
}  // namespace aidl
