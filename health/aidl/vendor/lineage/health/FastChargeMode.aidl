
/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package vendor.lineage.health;

@Backing(type="int") @VintfStability
enum FastChargeMode {
    /**
     * The device supports no fast charging.
     */
    NONE = 0,

    /**
     * The device supports fast charging.
     */
     FAST_CHARGE = 1,

     /**
      * The device supports super fast charging.
      */
     SUPER_FAST_CHARGE = 2,
}
