/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.chgctrl;

import vendor.lineage.chgctrl.ChargerPauseType;

@VintfStability
interface ICharger
{
    /**
     * Force set a pause charging state because of the user whats to charging to
     * pause.
     *
     * If for some reasons the user whats to charging to be paused now (for
     * example, during gaming in order to reduce heat), call this interface with
     * the argument being true. Internally the HAL will set (or clear) the
     * CHARGER_PAUSE_FORCE flags, to show that charging has been paused because
     * of the user specifically wants to pause charging now.
     */
    void setChargingPauseUser(in boolean enabled);

    /**
     * Get the internal pause charging reason in the HAL. The reasons are
     * defined in ChargerPauseType.aidl.
     *
     * Returns the current pause charging reason. The caller should use bit-wise
     * operations to determine the exact charging pause reason.
     */
    int getChargingPauseReasion();

    /**
     * Set a customized charging limit. The HAL will pause charging when
     * battery level larger than or equal to this limit.
     *
     * Internally, the HAL will be running a uevent loop at the background, and
     * when it finds uevents from power_supply SUBSYSTEM, it will read current
     * battery capacity from sysfs and compare with the limit set here. And
     * then call corresponding pause charging routines.
     */
    void setChargingLimit(in int limit);

    /**
     * Check current battery level and compare it with the limit set. If the
     * battery level is greater than or equal to the limit, stop charging and
     * set the corresponding charger pause reason bit (CHARGER_PAUSE_LIMIT).
     *
     * This will be called by the uevent loop once it receives uevents from
     * power_supply subsystem.
     */
    void checkBatCapacityAndApplyLimit();
}
