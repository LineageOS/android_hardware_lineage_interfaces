/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.slider;

import vendor.lineage.slider.SliderLocation;

/**
 * What a slider is, and where it sits on the device.
 */
@VintfStability
parcelable SliderInfo {
    /**
     * Number of discrete positions the slider has. At least 2, and constant
     * for the lifetime of the device. Indices increase from the slider's off
     * end to its on end. Devices must map their hardware so the off end
     * reports 0 and each further detent reports the next integer.
     */
    int positionCount;

    /**
     * Where the slider sits, or null when the device does not describe it.
     */
    @nullable SliderLocation location;
}
