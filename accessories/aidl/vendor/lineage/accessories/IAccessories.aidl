/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.accessories;

import vendor.lineage.accessories.IAccessory;
import vendor.lineage.accessories.IAccessoriesCallback;

/**
 * An interface for managing accessories.
 */
@VintfStability
interface IAccessories {
    /**
     * Get the current list of accessories.
     *
     * @return The current list of accessories
     */
    IAccessory[] getAccessories();

    /**
     * Set the callback that will receive accessory events.
     *
     * The callback will also receive all accessories through the
     * IAccessoriesCallback.onAccessoryAdded method during this call.
     *
     * @param callback The callback to register. If a previous callback was set, it will be
     *                 replaced. Can be null to remove the callback
     */
    oneway void setCallback(@nullable IAccessoriesCallback callback);
}
