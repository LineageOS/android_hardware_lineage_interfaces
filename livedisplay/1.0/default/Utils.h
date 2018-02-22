/*
** Copyright 2016, The CyanogenMod Project
**           2017, The LineageOS Project
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

#ifndef CYNGN_UTILS_H
#define CYNGN_UTILS_H

#include <stdlib.h>
#include <utils/Errors.h>

namespace android {

class Utils {

public:

    static status_t readInt(const char* node, int32_t* value);

    static status_t writeInt(const char* node, int32_t value);

    static status_t sendDPPSCommand(char* buf, size_t len);

    static status_t exists(const char* node);

    static status_t writeLocalModeId(int32_t id);

    static status_t readLocalModeId(int32_t* id);

    static status_t writeInitialModeId(int32_t id);

    static status_t readInitialModeId(int32_t* id);
};

};
#endif
