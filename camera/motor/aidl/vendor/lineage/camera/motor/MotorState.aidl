/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.camera.motor;

@VintfStability
@Backing(type="int")
enum MotorState {
    /**
     * The motor is down (equivalent of closed for the popup camera)
     */
    DOWN = 0,

    /**
     * The motor is up (equivalent of open for the popup camera)
     */
    UP = 1,

    /**
     * The motor needs calibration, the actual status should be considered as unreliable
     */
    NEEDS_CALIBRATION = 2,

    /**
     * The motor is calibrating: camera shouldn't be used until it's done
     */
    CALIBRATING = 3,
}
