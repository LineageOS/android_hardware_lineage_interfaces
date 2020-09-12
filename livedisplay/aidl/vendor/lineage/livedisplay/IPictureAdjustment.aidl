/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package vendor.lineage.livedisplay;

import vendor.lineage.livedisplay.FloatRange;
import vendor.lineage.livedisplay.HSIC;

@VintfStability
interface IPictureAdjustment {
    // Adding return type to method instead of out param FloatRange range since there is only one return value.
    FloatRange getHueRange();

    // Adding return type to method instead of out param FloatRange range since there is only one return value.
    FloatRange getSaturationRange();

    // Adding return type to method instead of out param FloatRange range since there is only one return value.
    FloatRange getIntensityRange();

    // Adding return type to method instead of out param FloatRange range since there is only one return value.
    FloatRange getContrastRange();

    // Adding return type to method instead of out param FloatRange range since there is only one return value.
    FloatRange getSaturationThresholdRange();

    // Adding return type to method instead of out param HSIC hsic since there is only one return value.
    HSIC getPictureAdjustment();

    // Adding return type to method instead of out param HSIC hsic since there is only one return value.
    HSIC getDefaultPictureAdjustment();

    // Adding return type to method instead of out param boolean rc since there is only one return value.
    boolean setPictureAdjustment(in HSIC hsic);
}
