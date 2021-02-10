/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package vendor.lineage.power;

import vendor.lineage.power.Boost;
import vendor.lineage.power.Feature;

@VintfStability
interface IPower {
    // Adding return type to method instead of out param int value since there is only one return value.
    /**
     * getFeature() is called to get the current value of a particular
     * feature or capability from the hardware or PowerHAL
     *
     * @return the state of power states the device supports
     * @param feature Feature which needs to get
     */
    int getFeature(in Feature feature);

    /**
     * setBoost() indicates the device may need to boost some resources, as the
     * the load is likely to increase before the kernel governors can react.
     * Depending on the boost, it may be appropriate to raise the frequencies of
     * CPU, GPU, memory subsystem, or stop CPU from going into deep sleep state.
     * A particular platform may choose to ignore this hint.
     *
     * @param type Boost type which is to be set with a timeout.
     * @param durationMs The expected duration of the user's interaction, if
     *        known, or 0 if the expected duration is unknown.
     *        a negative value indicates canceling previous boost.
     *        A given platform can choose to boost some time based on durationMs,
     *        and may also pick an appropriate timeout for 0 case.
     */
    oneway void setBoost(in Boost type, in int durationMs);
}
