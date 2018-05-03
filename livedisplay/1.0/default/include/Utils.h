/*
** Copyright 2016, The CyanogenMod Project
**           2017-2018, The LineageOS Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_UTILS_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_UTILS_H

#include <utils/Errors.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

class Utils {
  public:
    static android::status_t readInt(const char* node, int32_t* value);

    static android::status_t writeInt(const char* node, int32_t value);

    static android::status_t sendDPPSCommand(char* buf, size_t len);

    static android::status_t writeLocalModeId(int32_t id);

    static android::status_t readLocalModeId(int32_t* id);

    static android::status_t writeInitialModeId(int32_t id);

    static android::status_t readInitialModeId(int32_t* id);
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_UTILS_H
