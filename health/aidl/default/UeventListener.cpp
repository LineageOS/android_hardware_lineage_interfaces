/*
 * Copyright (C) 2018 The Android Open Source Project
 * Copyright (C) 2022 The LineageOS Project
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

#define LOG_TAG "vendor.lineage.health-uevent"

#include "UeventListener.h"
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <cutils/uevent.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

UeventListener::UeventListener() : uevent_fd_(-1) {}

std::shared_ptr<ICharger> getChargerService() {
    const std::string instance = std::string() + ICharger::descriptor + "/default";
    static bool isChargerDeclared = false;
    if (!isChargerDeclared) {
        // It is good to cache the result - it would not be changed
        isChargerDeclared = AServiceManager_isDeclared(instance.c_str());
        if (!isChargerDeclared) {
            LOG(ERROR) << "Charger service is not registered";
            return nullptr;
        }
    }

    return ICharger::fromBinder(ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
}

void UeventListener::handlePowerSupplyChange() {
    const std::shared_ptr<ICharger> charger_client = getChargerService();
    charger_client->checkBatCapacityAndApplyLimit();
}

#define UEVENT_MSG_LEN 2048
bool UeventListener::ProcessUevent() {
    char msg[UEVENT_MSG_LEN + 2];
    char* cp;
    int n;

    if (uevent_fd_ < 0) {
        uevent_fd_ = uevent_open_socket(64 * 1024, true);
        if (uevent_fd_ < 0) {
            LOG(ERROR) << "uevent_init: uevent_open_socket failed\n";
            return false;
        }
    }

    n = uevent_kernel_multicast_recv(uevent_fd_, msg, UEVENT_MSG_LEN);
    if (n <= 0 || n >= UEVENT_MSG_LEN) return false;

    // Ensure double-null termination of msg
    msg[n] = '\0';
    msg[n + 1] = '\0';

    /**
     * msg is a sequence of null-terminated strings.
     * Iterate through and record positions of string/value pairs of interest.
     * Double null indicates end of the message. (enforced above)
     */
    cp = msg;
    while (*cp) {
        if (!strncmp(cp, "SUBSYSTEM=power_supply", strlen("SUBSYSTEM=power_supply"))) {
            handlePowerSupplyChange();
            break;
        }

        while (*cp++)
            ;
    }

    return true;
};

void UeventListener::ListenForever() {
    constexpr int kMaxConsecutiveErrors = 10;
    int consecutive_errors = 0;

    while (1) {
        if (ProcessUevent()) {
            consecutive_errors = 0;
        } else {
            if (++consecutive_errors == kMaxConsecutiveErrors) {
                LOG(ERROR) << "Too many ProcessUevent errors; exiting UeventListener";
                return;
            }
        }
    }
}

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
