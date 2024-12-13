/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/radio/config/1.2/IRadioConfigIndication.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <lineage/hardware/radio/config/1.2/IRadioConfigIndication.h>

namespace lineage::hardware::radio::config::implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

class RadioConfigIndication : public V1_2::IRadioConfigIndication {
  public:
    // Methods from ::lineage::hardware::radio::config::V1_0::IRadioConfigIndication follow.
    Return<void> simSlotsStatusChanged(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::lineage::hardware::radio::config::V1_0::SimSlotStatus>& slotStatus)
            override;

    // Methods from ::lineage::hardware::radio::config::V1_2::IRadioConfigIndication follow.
    Return<void> simSlotsStatusChanged_1_2(
            ::android::hardware::radio::V1_0::RadioIndicationType type,
            const hidl_vec<::lineage::hardware::radio::config::V1_2::SimSlotStatus>& slotStatus)
            override;

    // Helper methods follow.
    void setResponseFunction(
            const sp<::android::hardware::radio::config::V1_0::IRadioConfigIndication>&
                    radioConfigIndication);

  private:
    sp<::android::hardware::radio::config::V1_0::IRadioConfigIndication> mRadioConfigIndication;
    sp<::android::hardware::radio::config::V1_2::IRadioConfigIndication> mRadioConfigIndicationV1_2;
};

}  // namespace lineage::hardware::radio::config::implementation
