/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.touch;

import vendor.lineage.touch.GestureType;

@VintfStability
parcelable Gesture {
    GestureType type;
    int keycode;
}
