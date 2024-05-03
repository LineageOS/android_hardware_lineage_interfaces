/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.input;

import vendor.lineage.input.BatteryStatus;

/**
 * Battery information for an accessory.
 */
@VintfStability
parcelable BatteryInfo {
    /**
     * Value used in DeviceInfo.batteryLevel when the battery level is unknown.
     */
    const int BATTERY_LEVEL_UNKNOWN = -1;

    /**
     * The current battery status.
     */
    BatteryStatus status = BatteryStatus.UNKNOWN;

    /**
     * The battery level of the device.
     * This value is in the range [0, 100] or BATTERY_LEVEL_UNKNOWN if unknown.
     */
    int levelPercentage = BatteryInfo.BATTERY_LEVEL_UNKNOWN;
}
