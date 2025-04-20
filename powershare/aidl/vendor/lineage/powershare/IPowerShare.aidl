/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.powershare;

@VintfStability
interface IPowerShare {
    // Returns the minimum battery level percentage required for PowerShare to be enabled.
    int getMinBattery();

    // Checks if PowerShare is currently enabled.
    boolean isEnabled();

    // Enables or disables PowerShare.
    void setEnabled(in boolean enable);

    // Sets the minimum battery level percentage required for PowerShare to function.
    void setMinBattery(in int minBattery);
}
