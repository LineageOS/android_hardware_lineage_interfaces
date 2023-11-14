/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.camera.motor;

import vendor.lineage.camera.motor.MotorState;

@VintfStability
interface ICameraMotor {
    /**
     * Get the current state of the motor.
     * @return MotorState
     */
    MotorState getState();

    /**
     * Open or close the camera popup.
     * @param state Either DOWN, UP or CALIBRATING. CALIBRATING should only be set if the service
     *              requires to do so after a user confirmation.
     */
    void setState(in MotorState state);

    /**
     * Try to quickly close the camera popup to protect the hardware (e.g. due to dropping the device).
     * If the hardware doesn't support this, proxy this call to setState(false).
     */
    void emergencyClose();
}
