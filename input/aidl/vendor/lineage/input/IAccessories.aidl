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
     */
    IAccessory[] getPersistentAccessories();

    /**
     * Register a callback to receive accessory events.
     * The callback will receive all dynamic devices through the onAccessoryAdded method during this
     * call.
     *
     * @param callback The callback to register.
     */
    void registerCallback(IAccessoriesCallback callback);

    /**
     * Unregister a callback.
     *
     * @param callback The callback to unregister.
     */
    void unregisterCallback(IAccessoriesCallback callback);
}
