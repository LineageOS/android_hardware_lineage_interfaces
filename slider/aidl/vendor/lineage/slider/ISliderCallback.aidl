/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.slider;

@VintfStability
interface ISliderCallback {
    /**
     * Invoked when the slider moves to a different position, and once when a
     * callback is registered to deliver the current position.
     *
     * @param position the current position index, in
     *                 [0, SliderInfo.positionCount).
     */
    oneway void onPositionChanged(int position);
}
