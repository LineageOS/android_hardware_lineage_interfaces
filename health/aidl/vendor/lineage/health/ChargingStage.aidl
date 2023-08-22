/*
 * Copyright (C) 2023 The StatiXOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.health;

import vendor.lineage.health.StageDescription;

@VintfStability
parcelable ChargingStage {
    /**
     * A description of the current stage of deadline charging.
     */
    StageDescription stage;

    /**
     * The seconds until the deadline is reached.
     */
    int deadlineSecs;
}
