/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/radio/1.4/IRadio.h>
#include <android/hardware/radio/1.4/IRadioResponse.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <lineage/hardware/radio/1.4/IRadio.h>

#define DEFINE_TO_LINEAGE_COPY(type)                                                              \
    static inline ::lineage::hardware::radio::type convert(::android::hardware::radio::type in) { \
        return static_cast<::lineage::hardware::radio::type>(in);                                 \
    }

#define DEFINE_TO_LINEAGE_REF(type)                                           \
    static inline const ::lineage::hardware::radio::type& convert(            \
            const ::android::hardware::radio::type& in) {                     \
        return reinterpret_cast<const ::lineage::hardware::radio::type&>(in); \
    }

#define DEFINE_TO_LINEAGE_HIDL_VEC_REF(type)                                                      \
    static inline const ::android::hardware::hidl_vec<::lineage::hardware::radio::type>& convert( \
            const ::android::hardware::hidl_vec<::android::hardware::radio::type>& in) {          \
        return reinterpret_cast<                                                                  \
                const ::android::hardware::hidl_vec<::lineage::hardware::radio::type>&>(in);      \
    }

namespace android::hardware::radio::implementation {

#define GSMBITS \
    (V1_0::RadioAccessFamily::GSM | V1_0::RadioAccessFamily::GPRS | V1_0::RadioAccessFamily::EDGE)
#define CDMABITS                                                       \
    (V1_0::RadioAccessFamily::IS95A | V1_0::RadioAccessFamily::IS95B | \
     V1_0::RadioAccessFamily::ONE_X_RTT)
#define EVDOBITS                                                         \
    (V1_0::RadioAccessFamily::EVDO_0 | V1_0::RadioAccessFamily::EVDO_A | \
     V1_0::RadioAccessFamily::EVDO_B | V1_0::RadioAccessFamily::EHRPD)
#define WCDMABITS                                                      \
    (V1_0::RadioAccessFamily::HSUPA | V1_0::RadioAccessFamily::HSDPA | \
     V1_0::RadioAccessFamily::HSPA | V1_0::RadioAccessFamily::HSPAP |  \
     V1_0::RadioAccessFamily::UMTS)
#define LTEBITS (V1_0::RadioAccessFamily::LTE | V1_0::RadioAccessFamily::LTE_CA)
#define TDSCDMABIT ((int)V1_0::RadioAccessFamily::TD_SCDMA)

V1_4::SignalStrength Create1_4SignalStrength(const V1_0::SignalStrength& sigStrength);
V1_4::SignalStrength Create1_4SignalStrength(const V1_2::SignalStrength& sigStrength);
android::hardware::hidl_vec<V1_4::CellInfo> Create1_4CellInfoList(
        const android::hardware::hidl_vec<V1_0::CellInfo>& cellInfo);
android::hardware::hidl_vec<V1_4::CellInfo> Create1_4CellInfoList(
        const android::hardware::hidl_vec<V1_2::CellInfo>& cellInfo);
V1_4::SetupDataCallResult Create1_4SetupDataCallResult(const V1_0::SetupDataCallResult& dcResponse);

DEFINE_TO_LINEAGE_REF(V1_0::CallForwardInfo)
DEFINE_TO_LINEAGE_REF(V1_0::CarrierRestrictions)
DEFINE_TO_LINEAGE_HIDL_VEC_REF(V1_0::CdmaBroadcastSmsConfigInfo)
DEFINE_TO_LINEAGE_COPY(V1_0::CdmaRoamingType)
DEFINE_TO_LINEAGE_REF(V1_0::CdmaSmsAck)
DEFINE_TO_LINEAGE_REF(V1_0::CdmaSmsMessage)
DEFINE_TO_LINEAGE_REF(V1_0::CdmaSmsWriteArgs)
DEFINE_TO_LINEAGE_COPY(V1_0::CdmaSubscriptionSource)
DEFINE_TO_LINEAGE_REF(V1_0::DataProfileInfo)
DEFINE_TO_LINEAGE_HIDL_VEC_REF(V1_0::DataProfileInfo)
DEFINE_TO_LINEAGE_COPY(V1_0::DeviceStateType)
DEFINE_TO_LINEAGE_REF(V1_0::Dial)
DEFINE_TO_LINEAGE_HIDL_VEC_REF(V1_0::GsmBroadcastSmsConfigInfo)
DEFINE_TO_LINEAGE_REF(V1_0::GsmSmsMessage)
DEFINE_TO_LINEAGE_REF(V1_0::IccIo)
DEFINE_TO_LINEAGE_REF(V1_0::ImsSmsMessage)
DEFINE_TO_LINEAGE_REF(V1_0::SimApdu)
DEFINE_TO_LINEAGE_COPY(V1_0::NvItem)
DEFINE_TO_LINEAGE_REF(V1_0::NvWriteItem)
DEFINE_TO_LINEAGE_COPY(V1_0::PreferredNetworkType)
DEFINE_TO_LINEAGE_COPY(V1_0::RadioBandMode)
DEFINE_TO_LINEAGE_REF(V1_0::RadioCapability)
DEFINE_TO_LINEAGE_COPY(V1_0::RadioTechnology)
DEFINE_TO_LINEAGE_COPY(V1_0::ResetNvType)
DEFINE_TO_LINEAGE_REF(V1_0::SelectUiccSub)
DEFINE_TO_LINEAGE_COPY(V1_0::SmsAcknowledgeFailCause)
DEFINE_TO_LINEAGE_REF(V1_0::SmsWriteArgs)
DEFINE_TO_LINEAGE_COPY(V1_0::TtyMode)
DEFINE_TO_LINEAGE_COPY(V1_1::CardPowerState)
DEFINE_TO_LINEAGE_REF(V1_1::ImsiEncryptionInfo)
DEFINE_TO_LINEAGE_REF(V1_1::KeepaliveRequest)
DEFINE_TO_LINEAGE_REF(V1_1::NetworkScanRequest)
DEFINE_TO_LINEAGE_HIDL_VEC_REF(V1_1::RadioAccessSpecifier)
DEFINE_TO_LINEAGE_COPY(V1_2::AccessNetwork)
DEFINE_TO_LINEAGE_COPY(V1_2::DataRequestReason)
DEFINE_TO_LINEAGE_REF(V1_2::NetworkScanRequest)
DEFINE_TO_LINEAGE_COPY(V1_4::AccessNetwork)
DEFINE_TO_LINEAGE_REF(V1_4::CarrierRestrictionsWithPriority)
DEFINE_TO_LINEAGE_REF(V1_4::DataProfileInfo)
DEFINE_TO_LINEAGE_HIDL_VEC_REF(V1_4::DataProfileInfo)
DEFINE_TO_LINEAGE_COPY(V1_4::EmergencyCallRouting)
DEFINE_TO_LINEAGE_COPY(V1_4::PdpProtocolType)
DEFINE_TO_LINEAGE_REF(V1_4::SimLockMultiSimPolicy)

}  // namespace android::hardware::radio::implementation
