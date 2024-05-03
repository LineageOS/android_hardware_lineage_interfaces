/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.input;

import vendor.lineage.input.IAccessory;
import vendor.lineage.input.IAccessoriesCallback;

/**
 * An interface for managing accessories.
 */
@VintfStability
interface IAccessories {
    /**
     * Get the list of accessories that won't disappear even if disconnected.
     *
     * To obtain the list of all accessories, use the callback.
     *
     * @return The list of persistent accessories
     */
    IAccessory[] getPersistentAccessories();

    /**
     * Set the callback that will receive accessory events.
     *
     * The callback will also receive all accessories through the
     * IAccessoriesCallback.onAccessoryAdded method during this call.
     *
     * @param callback The callback to register. If a previous callback was set, it will be
     *                 replaced. Can be null to remove the callback
     */
    void setCallback(@nullable IAccessoriesCallback callback);
}
