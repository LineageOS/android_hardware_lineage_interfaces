/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.slider;

/**
 * An edge of a display, named in its natural orientation.
 */
@VintfStability
@Backing(type="int")
enum Edge {
    LEFT,
    TOP,
    RIGHT,
    BOTTOM,
}
