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

#include "Utils.h"

#if !defined(__ANDROID__)
#include <fstab.h>
#endif
#include <sys/utsname.h>

namespace vendor {
namespace lineage {
namespace stache {
namespace V1_0 {
namespace implementation {

using ::android::NO_INIT;
using ::android::OK;
using ::android::status_t;

#if !defined(__ANDROID__)
status_t Utils::getFilesystemType(std::string& fsType, std::string mountPoint) {
    struct fstab *userdataFstab = getfsfile(mountPoint);
    if (userdataFstab == NULL) {
        return NO_INIT;
    }
    fsType = userdataFstab->fs_vfstype;
    return OK;
}
#endif

status_t Utils::getKernelVersion(std::string& kernelVersion) {
    struct utsname sysinfo;
    status_t ret = OK;
    int error = uname(&sysinfo);
    if (error == 0) {
        kernelVersion = sysinfo.release;
    }
    ret = error;
    return ret;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace stache
}  // namespace lineage
}  // namespace vendor
