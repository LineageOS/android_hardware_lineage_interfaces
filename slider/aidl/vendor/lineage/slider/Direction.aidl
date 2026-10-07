/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.slider;

/**
 * A direction across a display, named in its natural orientation.
 */
@VintfStability
@Backing(type="int")
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
}
