/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

@JavaDerive(toString=true) @VintfStability
parcelable FloatRange {
    float max;
    float min;
    float step;
}
