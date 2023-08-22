/*
 * Copyright (C) 2023 The StatiXOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.health;

@Backing(type="int")
@VintfStability
enum StageDescription {
    /**
     * Deadline charging is disabled.
     */
    DISABLED = 1 << 0,

    /**
     * The device is not currently charging to a deadline.
     */
    INACTIVE = 1 << 1,

    /**
     * Deadline charging is enabled.
     */
    ENABLED = 1 << 2,

    /**
     * The device is currently charging to a deadline.
     */
    ACTIVE = 1 << 3,
}
