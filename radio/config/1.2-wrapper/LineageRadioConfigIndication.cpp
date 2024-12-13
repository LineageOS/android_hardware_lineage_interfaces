/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "RadioConfigIndicationWrapper"

#include <android-base/logging.h>

#include "LineageRadioConfigIndication.h"

#define PROVIDE_RADIO_CONFIG_INDICATION_V1_2                                   \
    auto radioConfigIndicationV1_2 = mRadioConfigIndicationV1_2;               \
    if (radioConfigIndicationV1_2 == nullptr) {                                \
        LOG(ERROR) << __func__ << ": radioConfigIndicationV1_2 is null";       \
        return Status::fromExceptionCode(Status::Exception::EX_ILLEGAL_STATE); \
    }

namespace lineage::hardware::radio::config::implementation {

using ::android::hardware::Status;
using ::android::hardware::radio::V1_0::RadioIndicationType;

namespace {

::android::hardware::radio::config::V1_0::SimSlotStatus convertSimSlotStatus(
        const ::lineage::hardware::radio::config::V1_0::SimSlotStatus& slotStatus) {
    return {
            .cardState = slotStatus.cardState,
            .slotState = static_cast<::android::hardware::radio::config::V1_0::SlotState>(
                    slotStatus.slotState),
            .atr = slotStatus.atr,
            .logicalSlotId = slotStatus.logicalSlotId,
            .iccid = slotStatus.iccid,
    };
}

}  // namespace

// Methods from ::lineage::hardware::radio::config::V1_0::IRadioConfigIndication follow.
Return<void> RadioConfigIndication::simSlotsStatusChanged(
        RadioIndicationType type,
        const hidl_vec<::lineage::hardware::radio::config::V1_0::SimSlotStatus>& slotStatus) {
    PROVIDE_RADIO_CONFIG_INDICATION_V1_2

    std::vector<::android::hardware::radio::config::V1_2::SimSlotStatus> newSlotStatus;
    for (const auto& slotStatus_elem : slotStatus) {
        ::android::hardware::radio::config::V1_2::SimSlotStatus newSlotStatus_elem;
        newSlotStatus_elem.base = convertSimSlotStatus(slotStatus_elem);
        newSlotStatus.push_back(newSlotStatus_elem);
    }

    return radioConfigIndicationV1_2->simSlotsStatusChanged_1_2(type, newSlotStatus);
}

// Methods from ::lineage::hardware::radio::config::V1_2::IRadioConfigIndication follow.
Return<void> RadioConfigIndication::simSlotsStatusChanged_1_2(
        RadioIndicationType type,
        const hidl_vec<::lineage::hardware::radio::config::V1_2::SimSlotStatus>& slotStatus) {
    PROVIDE_RADIO_CONFIG_INDICATION_V1_2

    return radioConfigIndicationV1_2->simSlotsStatusChanged_1_2(
            type, reinterpret_cast<
                          const hidl_vec<::android::hardware::radio::config::V1_2::SimSlotStatus>&>(
                          slotStatus));
}

// Private methods follow.
void RadioConfigIndication::setResponseFunction(
        const sp<::android::hardware::radio::config::V1_0::IRadioConfigIndication>&
                radioConfigIndication) {
    mRadioConfigIndication = radioConfigIndication;
    mRadioConfigIndicationV1_2 =
            ::android::hardware::radio::config::V1_2::IRadioConfigIndication::castFrom(
                    mRadioConfigIndication)
                    .withDefault(nullptr);
}

}  // namespace lineage::hardware::radio::config::implementation
