/*
 * Copyright (C) 2016 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
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
#ifndef ANDROID_HARDWARE_USB_V1_1_USB_H
#define ANDROID_HARDWARE_USB_V1_1_USB_H

#include <android/hardware/usb/1.1/IUsb.h>
#include <android/hardware/usb/1.1/IUsbCallback.h>
#include <android/hardware/usb/1.1/types.h>
#include <hidl/Status.h>
#include <log/log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "android.hardware.usb@1.1-service.dual_role_usb"
#define UEVENT_MSG_LEN 2048

namespace android {
namespace hardware {
namespace usb {
namespace V1_1 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::usb::V1_0::PortDataRole;
using ::android::hardware::usb::V1_0::PortPowerRole;
using ::android::hardware::usb::V1_0::PortRole;
using ::android::hardware::usb::V1_0::PortRoleType;
using ::android::hardware::usb::V1_0::Status;
using ::android::hardware::usb::V1_1::IUsb;
using ::android::hardware::usb::V1_1::IUsbCallback;
using ::android::hardware::usb::V1_1::PortMode_1_1;
using ::android::hardware::usb::V1_1::PortStatus_1_1;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;

struct Usb : public IUsb {
    Usb();
    Return<void> switchRole(const hidl_string& portName, const PortRole& role) override;
    Return<void> setCallback(const sp<V1_0::IUsbCallback>& callback) override;
    Return<void> queryPortStatus() override;

    sp<V1_0::IUsbCallback> mCallback_1_0;
    pthread_t mPoll;
    // Protects mCallback variable
    pthread_mutex_t mLock = PTHREAD_MUTEX_INITIALIZER;
    // Protects roleSwitch operation
    pthread_mutex_t mRoleSwitchLock = PTHREAD_MUTEX_INITIALIZER;
};

}  // namespace implementation
}  // namespace V1_1
}  // namespace usb
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_USB_V1_1_USB_H
