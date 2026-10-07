/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>
#include <string>
#include <vector>

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

// Opens the first input device carrying one of these names that accept()
// takes, which is where a caller decides whether a device reports what it
// needs. Returns -1 when none does.
int openInputDevice(const std::vector<std::string>& names, bool verbose,
                    const std::function<bool(int fd, const std::string& name)>& accept);

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
