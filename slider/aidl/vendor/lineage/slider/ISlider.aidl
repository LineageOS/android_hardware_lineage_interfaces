/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.slider;

import vendor.lineage.slider.ISliderCallback;
import vendor.lineage.slider.SliderInfo;

/**
 * A physical multi-position slider (e.g. a mode or alert slider).
 *
 * The interface describes only abstract positions. Nothing about how a position
 * is sensed (input device, key or switch codes, axis, sysfs node) is exposed,
 * which is an implementation concern. Each physical slider is a separate service
 * instance, identified by its instance name.
 */
@VintfStability
interface ISlider {
    /**
     * What this slider is and where it sits, which is constant for the
     * lifetime of the device.
     *
     * @return the slider's description.
     */
    SliderInfo getSliderInfo();

    /**
     * Current position index, in [0, SliderInfo.positionCount).
     *
     * An implementation must back this with a stateful source, so the value is
     * valid at boot and after a service restart without waiting for the slider
     * to move.
     *
     * @return the current position index.
     */
    int getPosition();

    /**
     * Register a callback for position changes. On registration the current
     * position is delivered once via {@link ISliderCallback#onPositionChanged},
     * and a registration whose delivery fails returns that failure.
     *
     * Registrations do not survive a restart of the service, so a client that
     * must keep receiving changes links to its death and registers again.
     *
     * @param callback the callback to register.
     */
    void registerCallback(ISliderCallback callback);

    /**
     * Unregister a previously registered callback. A callback that is not
     * registered is ignored.
     *
     * @param callback the callback to unregister.
     */
    void unregisterCallback(ISliderCallback callback);
}
