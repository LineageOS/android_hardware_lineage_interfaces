/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "DisplayColorCalibrationService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <livedisplay/sysfs/DisplayColorCalibration.h>

using ::android::base::ReadFileToString;
using ::android::base::Split;
using ::android::base::StringPrintf;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

static constexpr const char* kDisplayColorCalibrationPaths[] = {
        "/sys/class/graphics/fb0/rgb",
};

DisplayColorCalibration::DisplayColorCalibration() {
    file_ = nullptr;
    for (const auto& path : kDisplayColorCalibrationPaths) {
        if (!access(path, R_OK | W_OK)) {
            file_ = path;
            break;
        }
    }
}

ndk::ScopedAStatus DisplayColorCalibration::getMaxValue(int32_t* _aidl_return) {
    *_aidl_return = 32768;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayColorCalibration::getMinValue(int32_t* _aidl_return) {
    *_aidl_return = 255;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayColorCalibration::getCalibration(std::vector<int32_t>* _aidl_return) {
    if (file_ == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::vector<int32_t> rgb;
    std::string tmp;

    if (ReadFileToString(file_, &tmp)) {
        for (auto&& color : Split(Trim(tmp), " ")) {
            rgb.push_back(std::stoi(color));
        }
    }

    *_aidl_return = rgb;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayColorCalibration::setCalibration(const std::vector<int32_t>& rgb) {
    if (file_ == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (rgb.size() != 3) {
        LOG(ERROR) << "Unrecognized RGB data!";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    if (!WriteStringToFile(StringPrintf("%d %d %d", rgb[0], rgb[1], rgb[2]), file_, true)) {
        LOG(ERROR) << "Failed to set DisplayColorCalibration state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
