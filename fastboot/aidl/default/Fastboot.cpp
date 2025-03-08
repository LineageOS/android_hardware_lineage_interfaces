/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include "Fastboot.h"

#include <android-base/properties.h>
#include <android-base/strings.h>

#include <unordered_map>
#include <vector>

using ndk::ScopedAStatus;

namespace aidl {
namespace android {
namespace hardware {
namespace fastboot {

using OEMCommandHandler =
        std::function<ScopedAStatus(const std::vector<std::string>&, std::string*)>;

ScopedAStatus Fastboot::getPartitionType(const std::string& in_partitionName,
                                         FileSystemType* _aidl_return) {
    if (in_partitionName.empty()) {
        return ScopedAStatus::fromExceptionCodeWithMessage(EX_ILLEGAL_ARGUMENT,
                                                           "Invalid partition name");
    }
    *_aidl_return = FileSystemType::RAW;
    return ScopedAStatus::ok();
}

ScopedAStatus GetProp(const std::vector<std::string>& args, std::string* _aidl_return) {
    if (!args.size()) {
        return ScopedAStatus::fromExceptionCodeWithMessage(EX_ILLEGAL_ARGUMENT,
                                                           "Property unspecified");
    }

    auto property = ::android::base::GetProperty(args[0], "");

    if (!property.empty()) {
        *_aidl_return = args[0] + ": " + property;
        return ScopedAStatus::ok();
    }

    return ScopedAStatus::fromServiceSpecificErrorWithMessage(BnFastboot::FAILURE_UNKNOWN,
                                                              "Unable to get property");
}

ScopedAStatus Fastboot::doOemCommand(const std::string& in_oemCmd, std::string* _aidl_return) {
    const std::unordered_map<std::string, OEMCommandHandler> kOEMCmdMap = {
            {FB_OEM_GET_PROP, GetProp},
    };

    auto args = ::android::base::Split(in_oemCmd, " ");
    if (args.size() < 2) {
        return ScopedAStatus::fromExceptionCodeWithMessage(EX_ILLEGAL_ARGUMENT,
                                                           "Invalid OEM command");
    }

    // args[0] will be "oem", args[1] will be the command name
    auto cmd_handler = kOEMCmdMap.find(args[1]);
    if (cmd_handler != kOEMCmdMap.end()) {
        return cmd_handler->second(std::vector<std::string>(args.begin() + 2, args.end()),
                                   _aidl_return);
    } else {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(BnFastboot::FAILURE_UNKNOWN,
                                                                  "Unknown OEM Command");
    }

    return ScopedAStatus::ok();
}

ScopedAStatus Fastboot::getVariant(std::string* _aidl_return) {
    *_aidl_return = "NA";
    return ScopedAStatus::ok();
}

ScopedAStatus Fastboot::getOffModeChargeState(bool* _aidl_return) {
    *_aidl_return = false;
    return ScopedAStatus::ok();
}

ScopedAStatus Fastboot::getBatteryVoltageFlashingThreshold(int32_t* _aidl_return) {
    *_aidl_return = 0;
    return ScopedAStatus::ok();
}

ScopedAStatus Fastboot::doOemSpecificErase() {
    return ScopedAStatus::fromExceptionCodeWithMessage(
            EX_UNSUPPORTED_OPERATION, "Command not supported in default implementation");
}

}  // namespace fastboot
}  // namespace hardware
}  // namespace android
}  // namespace aidl
