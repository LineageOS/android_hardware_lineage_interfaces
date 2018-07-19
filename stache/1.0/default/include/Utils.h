/*
 * Copyright (C) 2018 The LineageOS Project
 * Author: Rashed Abdel-Tawab (rashed@linux.com)
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

#ifndef VENDOR_LINEAGE_STACHE_V1_0_UTILS_H
#define VENDOR_LINEAGE_STACHE_V1_0_UTILS_H

#include <string>
#include <utils/Errors.h>

namespace vendor {
namespace lineage {
namespace stache {
namespace V1_0 {
namespace implementation {

class Utils {
  public:
#if !defined(__ANDROID__)
    static android::status_t getFilesystemType(std::string& fsType, std::string mountPoint);
#endif

    static android::status_t getKernelVersion(std::string& kernelVersion);
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace stache
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_STACHE_V1_0_UTILS_H
