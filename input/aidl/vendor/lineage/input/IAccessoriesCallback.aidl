/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.input;

import vendor.lineage.input.AccessoryInfo;
import vendor.lineage.input.AccessoryStatus;
import vendor.lineage.input.BatteryInfo;
import vendor.lineage.input.IAccessory;

/**
 * Callback for accessories information updates.
 */
@VintfStability
interface IAccessoriesCallback {
    /**
     * Called when a dynamic accessory is added.
     *
     * This method is only called for non persistent accessories, persistent accessories will get
     * connection updates through the onAccessoryInfoUpdated method.
     *
     * @param accessoryInfo The information of the device that was added
     * @param accessory The accessory interface
     */
    void onAccessoryAdded(in AccessoryInfo accessoryInfo, IAccessory accessory);

    /**
     * Called when an accessory is removed. The accessory must be removed from the list of
     * accessories.
     *
     * This method is only called for non persistent accessories, persistent accessories will get
     * connection updates through the onAccessoryInfoUpdated method.
     *
     * @param accessoryInfo The information of the device that was removed
     */
    void onAccessoryRemoved(in AccessoryInfo accessoryInfo);

    /**
     * Called when an accessory's information is updated.
     *
     * @param accessoryInfo The updated information of the device
     */
    void onAccessoryInfoUpdated(in AccessoryInfo accessoryInfo);

    /**
     * Called when an accessory's status is updated.
     *
     * @param accessoryInfo The ID of the device that was updated
     * @param status The updated status of the device
     */
    void onAccessoryStatusUpdated(in AccessoryInfo accessoryInfo, AccessoryStatus status);

    /**
     * Called when an accessory's battery information is updated.
     *
     * @param accessoryId The ID of the device that was updated
     * @param batteryInfo The updated battery information of the device
     */
    void onBatteryInfoUpdated(in AccessoryInfo accessoryInfo, in BatteryInfo batteryInfo);
}
