/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.health;

@VintfStability
interface IChargingControl
{
    /**
     * Get battery charging enablement status.
     * Note that this only returns whether charging should be enabled or not when a charger
     * is plugged, not if the battery is actually charging.
     *
     * @return whether if battery charging is enabled.
     *         If error:
     *         - Return exception with code EX_ILLEGAL_STATE
     *           if the value can't be determined from the driver.
     *         - Return service specific error with code STATUS_UNKNOWN
     *           for other errors.
     */
    boolean getChargingEnabled();

    /**
     * Set battery charging enabled.
     *
     * @return nothing if successful.
     *         If error:
     *         - Return service specific error with code STATUS_UNKNOWN
     *           for other errors.
     */
    void setChargingEnabled(in boolean enabled);

    /**
     * Get whether the device bypasses battery when charging stops. If the device
     * supports this feature, the amperage will be 0 when the charging is stopped.
     *
     * @return Whether the device supports charging bypass.
     */
     boolean deviceChargingControlBypassesBattery();
}
