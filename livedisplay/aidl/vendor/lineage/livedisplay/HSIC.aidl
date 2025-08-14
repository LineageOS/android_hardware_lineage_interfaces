/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

@JavaDerive(toString=true) @VintfStability
parcelable HSIC {
    float hue;
    float saturation;
    float intensity;
    float contrast;
    float saturationThreshold;
}
