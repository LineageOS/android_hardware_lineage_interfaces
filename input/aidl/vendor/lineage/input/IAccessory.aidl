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
 * Interface for interacting with an accessory.
 */
@VintfStability
interface IAccessory {
    /**
     * Get the accessory information.
     *
     * @return The accessory information
     */
    AccessoryInfo getAccessoryInfo();

    /**
     * Get the current accessory status.
     *
     * @return The accessory status
     */
    AccessoryStatus getAccessoryStatus();

    /**
     * Get the battery information
     *
     * @return The battery information
     */
    BatteryInfo getBatteryInfo();

    /**
     * Return true if the accessory is enabled.
     *
     * @return True if the accessory is enabled
     */
    boolean isEnabled();

    /**
     * Set the accessory enabled state.
     *
     * @param enabled True to enable the accessory, false to disable it
     */
    void setEnabled(boolean enabled);
}
