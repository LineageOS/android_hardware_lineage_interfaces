/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/health/BnFastCharge.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

struct FastCharge : public BnFastCharge {
  public:
    ndk::ScopedAStatus getSupportedFastChargeModes(int64_t* _aidl_return) override;
    ndk::ScopedAStatus getFastChargeMode(FastChargeMode* _aidl_return) override;
    ndk::ScopedAStatus setFastChargeMode(FastChargeMode in_mode,
                                         FastChargeMode* _aidl_return) override;

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;
};

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
