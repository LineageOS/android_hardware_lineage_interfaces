/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
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

constexpr size_t kBitsPerWord = sizeof(unsigned long) * 8;

// How many words hold a bitmap of the kind EVIOCGBIT, EVIOCGKEY and EVIOCGSW
// fill, which is one bit per code.
constexpr size_t bitWords(size_t codes) {
    return (codes + kBitsPerWord - 1) / kBitsPerWord;
}

bool isBitSet(const unsigned long* bits, int code);

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
