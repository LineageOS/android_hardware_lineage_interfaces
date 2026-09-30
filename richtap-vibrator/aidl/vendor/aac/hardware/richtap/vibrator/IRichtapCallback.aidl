/*
 * Copyright (C) 2021 The Android AAC vibration extension
 * Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.aac.hardware.richtap.vibrator;

@VintfStability
interface IRichtapCallback {
    oneway void onCallback(in int status);
}