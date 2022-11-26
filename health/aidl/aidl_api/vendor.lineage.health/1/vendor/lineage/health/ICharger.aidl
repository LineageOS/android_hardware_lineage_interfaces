/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.health;

import vendor.lineage.health.ChargerPauseType;

@VintfStability
interface ICharger
{
    /**
     * Force set a pause charging state because the user wants the charging to
     * pause.
     *
     * If for some reasons the user wants the charging to pause now (For
     * example, during gaming in order to reduce heat), call this interface with
     * the argument being true. Internally the HAL will set (or clear) the
     * CHARGER_PAUSE_FORCE flag, to show that charging has been paused because
     * of the user request.
     */
    void setChargingPauseUser(in boolean enabled);

    /**
     * Get the internal pause charging reason in the HAL. The reasons are
     * defined in ChargerPauseType.aidl.
     *
     * Returns the current pause charging reason. The caller should use bit-wise
     * operations to determine the exact charging pause reason.
     */
    int getChargingPauseReason();

    /**
     * Set a custom charging limit. The HAL will pause charging when the battery
     * level is larger than or equal to this limit.
     *
     * Internally, the HAL will be running a uevent loop in the background, and
     * when it receives uevents from power_supply SUBSYSTEM, it will read the
     * current battery capacity from sysfs and compare it with the limit set
     * here. And then call corresponding pause charging routines.
     */
    void setChargingLimit(in int limit);

    /**
     * Check current battery level and compare it with the limit set. If the
     * battery level is greater than or equal to the limit, pause charging and
     * set the corresponding charging pause reason (CHARGER_PAUSE_LIMIT).
     */
    void checkBatCapacityAndApplyLimit();
}
