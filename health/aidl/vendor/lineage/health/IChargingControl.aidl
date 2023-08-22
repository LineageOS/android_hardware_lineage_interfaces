/*
 * Copyright (C) 2022-2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.health;

import vendor.lineage.health.ChargingStage;

@VintfStability
interface IChargingControl {
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
     * Sets the charging control deadline, if deadline mode is supported.
     *
     * @return nothing if successful.
     *         If error:
     *         - Return service specific error with code STATUS_UNKNOWN
     *            for other errors.
     */
    void setChargingDeadline(in long deadline);

    /**
     * Get the device's supported charging control mode.
     *
     * @return a bitmask of ChargingControlSupportedMode.
     */
    int getSupportedMode();

    /**
     * Get the device's current charging stage and deadline.
     *
     * @return ChargingStage describing the current active state and deadline in
     *         seconds from now.
     */
    ChargingStage getChargingStageAndDeadline();
}
