/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <models/State.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

bool State::isLit() {
    return color.isLit();
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
