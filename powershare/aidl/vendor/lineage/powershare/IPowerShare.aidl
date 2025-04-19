/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.powershare;

@VintfStability
interface IPowerShare {
    // Adding return type to method instead of out param int rc since there is only one return value.
    int getMinBattery();

    // Adding return type to method instead of out param boolean rc since there is only one return value.
    boolean isEnabled();

    // Adding return type to method instead of out param boolean rc since there is only one return value.
    boolean setEnabled(in boolean enable);

    // Adding return type to method instead of out param int rc since there is only one return value.
    int setMinBattery(in int minBattery);
}
