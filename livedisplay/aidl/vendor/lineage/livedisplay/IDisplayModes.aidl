/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

import vendor.lineage.livedisplay.DisplayMode;

@VintfStability
interface IDisplayModes {
    DisplayMode[] getDisplayModes();
    DisplayMode getCurrentDisplayMode();
    DisplayMode getDefaultDisplayMode();
    void setDisplayMode(in int modeID, in boolean makeDefault);
}
