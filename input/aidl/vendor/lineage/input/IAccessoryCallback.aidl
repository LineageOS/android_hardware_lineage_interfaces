/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.input;

import vendor.lineage.input.AccessoryInfo;
import vendor.lineage.input.AccessoryStatus;
import vendor.lineage.input.BatteryInfo;

/**
 * Callback for accessory information updates.
 */
@VintfStability
interface IAccessoryCallback {
    /**
     * Called when the accessory's information is updated.
     *
     * @param accessoryInfo The updated information of the device
     */
    void onAccessoryInfoUpdated(in AccessoryInfo accessoryInfo);

    /**
     * Called when the accessory's status is updated.
     *
     * @param status The updated status of the device
     */
    void onAccessoryStatusUpdated(AccessoryStatus status);

    /**
     * Called when the accessory's battery information is updated.
     *
     * @param batteryInfo The updated battery information of the device, can return null if the
     *                    battery information became unavailable due to a change of state
     */
    void onBatteryInfoUpdated(in @nullable BatteryInfo batteryInfo);
}
