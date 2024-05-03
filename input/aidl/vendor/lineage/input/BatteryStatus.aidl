/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.input;

/**
 * Device's battery status
 */
@VintfStability
enum BatteryStatus {
    /**
     * The battery status is unknown.
     */
    UNKNOWN,

    /**
     * The battery is charging.
     * Battery flow is >0mA.
     */
    CHARGING,

    /**
     * The battery is discharging.
     * Battery flow is <0mA.
     */
    DISCHARGING,

    /**
     * The device is not charging its battery.
     * Battery flow is ~0mA.
     */
    NOT_CHARGING,
}
