/*
 * SPDX-FileCopyrightText: 2017-2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "ConsumerIr"

#include "ConsumerIr.h"

#include <ir.sysprop.h>

#include <android-base/logging.h>
#include <fcntl.h>
#include <linux/lirc.h>
#include <string>

using std::vector;

using namespace ::vendor::lineage::ir;

namespace aidl {
namespace android {
namespace hardware {
namespace ir {

static const std::string kIrDevice = "/dev/lirc0";

static vector<ConsumerIrFreqRange> kRangeVec;

ConsumerIr::ConsumerIr() {
    auto carrier_freqs = IrProperties::carrier_freqs();

    if (carrier_freqs.size() >= 2) {
        for (size_t i = 0; i < carrier_freqs.size() - 1; i += 2) {
            if (!carrier_freqs[i] || !carrier_freqs[i + 1]) {
                continue;
            }

            kRangeVec.push_back({
                    .minHz = carrier_freqs[i].value(),
                    .maxHz = carrier_freqs[i + 1].value(),
            });
        }
    } else {
        kRangeVec.push_back({.minHz = 30000, .maxHz = 60000});
    }
}

::ndk::ScopedAStatus ConsumerIr::getCarrierFreqs(vector<ConsumerIrFreqRange>* _aidl_return) {
    *_aidl_return = kRangeVec;

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus ConsumerIr::transmit(int32_t carrierFreqHz, const vector<int32_t>& pattern) {
    size_t entries = pattern.size();

    if (entries == 0) {
        return ::ndk::ScopedAStatus::ok();
    }

    int fd = open(kIrDevice.c_str(), O_RDWR);
    if (fd < 0) {
        LOG(ERROR) << "Failed to open " << kIrDevice << ", error " << fd;

        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    int rc = ioctl(fd, LIRC_SET_SEND_CARRIER, &carrierFreqHz);
    if (rc < 0) {
        LOG(ERROR) << "Failed to set carrier " << carrierFreqHz << ", error: " << errno;

        close(fd);

        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if ((entries & 1) != 0) {
        rc = write(fd, pattern.data(), entries * sizeof(int32_t));
    } else {
        rc = write(fd, pattern.data(), (entries - 1) * sizeof(int32_t));
    }

    if (rc < 0) {
        LOG(ERROR) << "Failed to write pattern, " << entries << " entries, error: " << errno;

        close(fd);

        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    close(fd);

    return ::ndk::ScopedAStatus::ok();
}

}  // namespace ir
}  // namespace hardware
}  // namespace android
}  // namespace aidl
