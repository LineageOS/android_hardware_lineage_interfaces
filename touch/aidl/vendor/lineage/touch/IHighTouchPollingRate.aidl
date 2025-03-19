/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.touch;

@VintfStability
interface IHighTouchPollingRate {
    void setEnabled(in boolean enabled);
    boolean getEnabled();
}
