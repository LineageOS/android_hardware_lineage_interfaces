/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.health;

@VintfStability
interface ILineageHealth
{
    /**
     * Get battery charging enabled.
     * Note that this doesn't return if the battery is actually charging 
     * but only if charging should be enabled or not when a charger is plugged.
     *
     * @return wherever if battery charging is enabled.
     *         If error:
     *         - Return exception with code EX_UNSUPPORTED_OPERATION
     *           if toggling charging is not supported.
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
     *         - Return exception with code EX_UNSUPPORTED_OPERATION
     *           if toggling charging is not supported.
     *         - Return service specific error with code STATUS_UNKNOWN
     *           for other errors.
     */
    void setChargingEnabled(in boolean enabled);
}
