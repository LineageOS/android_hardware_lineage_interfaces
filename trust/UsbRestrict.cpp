/*
 * Copyright (C) 2019 The LineageOS Project
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
#include <fstream>

#include "UsbRestrict.h"

namespace vendor {
namespace lineage {
namespace trust {
namespace V1_0 {
namespace implementation {

UsbRestrict::UsbRestrict() {
}

// Methods from ::vendor::lineage::trust::V1_0::IUsbRestrict follow.
Return<bool> UsbRestrict::isEnabled() {
    std::ifstream file("/proc/sys/kernel/deny_new_usb");
    std::string content;
    file >> content;
    return !file.fail() && std::stoi(content);
}

Return<bool> UsbRestrict::setEnabled(bool enabled) {
    std::ofstream file("/proc/sys/kernel/deny_new_usb");
    file << (enabled ? "1" : "0");
    file.close();
    return true;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IUsbRestrict* HIDL_FETCH_IUsbRestrict(const char* /* name */) {
    //return new UsbRestrict();
//}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace trust
}  // namespace lineage
}  // namespace vendor
