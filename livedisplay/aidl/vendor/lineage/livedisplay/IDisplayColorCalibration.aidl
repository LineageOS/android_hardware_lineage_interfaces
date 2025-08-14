/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

@VintfStability
interface IDisplayColorCalibration {
    int getMaxValue();
    int getMinValue();
    int[] getCalibration();
    boolean setCalibration(in int[] rgb);
}
