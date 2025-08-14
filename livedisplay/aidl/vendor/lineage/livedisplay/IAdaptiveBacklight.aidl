/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

@VintfStability
interface IAdaptiveBacklight {
    boolean getEnabled();
    boolean setEnabled(in boolean enabled);
}
