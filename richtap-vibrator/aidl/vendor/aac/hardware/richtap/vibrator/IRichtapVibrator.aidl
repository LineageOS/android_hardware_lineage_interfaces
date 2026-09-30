/*
 * Copyright (C) 2021 The Android AAC vibration extension
 * Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.aac.hardware.richtap.vibrator;

import vendor.aac.hardware.richtap.vibrator.IRichtapCallback;

@VintfStability
interface IRichtapVibrator {

    /**
     * init richtap
     *
     * @param callback callback for result
     */
    oneway void init(in IRichtapCallback callback);

    /**
     * setting pattern mode dynamic scale
     *
     * @param scale value (from 0 to 100)
     * @return whether setDynamicScale was successful or not.
     */
    oneway void setDynamicScale(in int scale,in IRichtapCallback callback);

    /**
     * setting f0 param
     *
     * @param f0 value
     * @return whether vibrator command was successful or not.
     */
    oneway void setF0(in int f0,in IRichtapCallback callback);

    /**
     * stop interface for short latency
     * @param callback callback for result
     */
    oneway void stop(in IRichtapCallback callback);

    /**
     * setAmplitude interface for change amplitude
     * @param amplitude amplitude value
     * @param callback callback for result
     */
    oneway void setAmplitude(in int amplitude,in IRichtapCallback callback);

    /**
     * he mode interface
     *
     * @param interval interval value
     * @param amplitude amplitude value
     * @param freq frequency value
     * @return whether vibrator command was successful or not.
     */
    oneway void performHeParam(in int interval, in int amplitude, in int freq,in IRichtapCallback callback);

    /**
     * off interface
     */
    oneway void off(in IRichtapCallback callback);

    /**
     * vibrator on
     *
     * @param timeoutMs timeout in ms
     * @return cmd id.
     */
    oneway void on(in int timeoutMs,in IRichtapCallback callback);

    /**
     * perform prebaked
     *
     * @param effect_id strength
     * @return cmd id.
     */
    int perform(in int effect_id, in byte strength,in IRichtapCallback callback);

    /**
     * envelope parameter generate data
     *
     * @param envInfo envelope parameter data
     * @param fastFlag fast flag
     * @return cmd id.
     */
    oneway void performEnvelope(in int[] envInfo, in boolean fastFlag,in IRichtapCallback callback);

    /**
     * output based on voltage or accelerate signal
     * @param hdl file descriptor for waveform data
     */
    oneway void performRtp(in ParcelFileDescriptor hdl,in IRichtapCallback callback);

    /**
     * he mode interface
     *
     * @param looper looper count
     * @param interval interval value
     * @param amplitude amplitude value
     * @param freq frequency value
     * @param he HE parameter data
     * @return cmd id.
     */
    oneway void performHe(in int looper, in int interval, in int amplitude, in int freq, in int[] he,in IRichtapCallback callback);

    /**
     * interface that's for DRC parameters setting, etc.
     *
     * @param data parameters
     * @param length length of parameters.
     */
    oneway void setHapticParam(in int[] data, in int length, in IRichtapCallback callback);
}
