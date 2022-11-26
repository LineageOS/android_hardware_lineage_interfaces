/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.chgctrl;

/**
 * Different charging pause reasions for Charger HAL
 */
@VintfStability
@Backing(type="int")
enum ChargerPauseType {
    /**
     * Users specifically require the charging to pause.
     */
    CHARGER_PAUSE_FORCE = 0x1 << 0,

    /**
     * Charging has suspended because the battery level has reached the
     * charging limit that has been specified by user.
     */
    CHARGER_PAUSE_LIMIT = 0x1 << 1,
}
