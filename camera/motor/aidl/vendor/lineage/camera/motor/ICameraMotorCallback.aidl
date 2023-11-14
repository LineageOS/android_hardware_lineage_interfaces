/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.camera.motor;

import vendor.lineage.camera.motor.MotorState;

@VintfStability
interface ICameraMotorCallback {
    /**
     * Called when the motor state changes.
     *
     * @param state The new motor state.
     */
    void onStateChanged(MotorState state);
}
