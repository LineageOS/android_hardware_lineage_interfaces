/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

@VintfStability
interface IAdaptiveBacklight {
    boolean getEnabled();
    void setEnabled(in boolean enabled);
}
