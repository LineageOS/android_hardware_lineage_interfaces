/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.input;

import vendor.lineage.input.AccessoryType;
import vendor.lineage.input.BatteryInfo;

/**
 * Information about an accessory.
 */
@VintfStability
parcelable AccessoryInfo {
    /**
     * A unique identifier for this accessory. This ID will be used to interact with the accessory
     * and call methods on it.
     * This ID must be stable per device across reboots, since it will be used to also store user
     * preferences.
     * It might be a good idea to namespace the ID with the following format, all lowercase:
     * <device type>:<connection protocol>:<connection unique identifier>
     * For example, a USB keyboard might use the following ID:
     * keyboard:usb:1234:5678
     * An accessory connected to the device using a non-standard protocol could use:
     * pen:internal:1
     */
    String id;

    /**
     * The type of the device.
     */
    AccessoryType type;

    /**
     * Whether the accessory is persistent.
     * If true, the accessory should be listed even if disconnected.
     */
    boolean isPersistent;

    /**
     * A user friendly name for the accessory, either a specific name specified by the user or the
     * brand and model name of the device.
     */
    String displayName;

    /**
     * A display identifier for this accessory.
     * For example, a USB device might use the USB vendor and product ID as the identifier.
     * A Bluetooth device might use the Bluetooth MAC address as the identifier.
     * If there's no unique identifier for the device, this field can be left empty.
     */
    String displayId = "";

    /**
     * Whether the accessory has a battery.
     * If true, IAccessory.getBatteryInfo() should return a valid BatteryInfo object on a compatible
     * mode.
     */
    boolean hasBattery;
}
