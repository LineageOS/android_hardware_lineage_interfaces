/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.livedisplay;

import vendor.lineage.livedisplay.Range;

@VintfStability
interface IColorBalance {
    Range getColorBalanceRange();
    int getColorBalance();
    void setColorBalance(in int value);
}
