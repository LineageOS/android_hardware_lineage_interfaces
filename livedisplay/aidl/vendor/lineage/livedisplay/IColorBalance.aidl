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

import vendor.lineage.livedisplay.Range;

@VintfStability
interface IColorBalance {
    // Adding return type to method instead of out param Range range since there is only one return value.
    Range getColorBalanceRange();

    // Adding return type to method instead of out param int value since there is only one return value.
    int getColorBalance();

    // Adding return type to method instead of out param boolean rc since there is only one return value.
    boolean setColorBalance(in int value);
}
