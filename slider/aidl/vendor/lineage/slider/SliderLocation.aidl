/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.slider;

import vendor.lineage.slider.Direction;
import vendor.lineage.slider.Edge;

/**
 * Where a slider sits on the device.
 */
@VintfStability
parcelable SliderLocation {
    /**
     * The display edge the slider sits beside.
     */
    Edge edge;

    /**
     * The way the slider travels as the position index rises, named in the
     * display's natural orientation. It runs along the edge the slider sits
     * beside, so a LEFT or RIGHT edge carries UP or DOWN, and a TOP or
     * BOTTOM edge carries LEFT or RIGHT.
     *
     * A client listing the positions in the order they are mounted follows
     * this rather than the index order.
     */
    Direction direction;

    /**
     * Where along that edge the middle of the slider's travel is, in pixels
     * from the origin of the display in its natural orientation. For LEFT and
     * RIGHT it is measured down the display, and for TOP and BOTTOM across it.
     *
     * Measured at the largest resolution the display reports as supported,
     * which is the one the panel is built at. A client drawing on a display
     * driven at a smaller mode scales this by the ratio between the two.
     */
    int offsetPixels;

    /**
     * The display the offset is measured on, as
     * android.view.Display#getUniqueId reports it. An empty string names the
     * default display.
     */
    String display = "";
}
