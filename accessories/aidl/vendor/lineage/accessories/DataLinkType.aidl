/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.accessories;

/**
 * The link used to exchange data with an accessory.
 */
@VintfStability
enum DataLinkType {
    /**
     * Bluetooth Low Energy. The ID is the Bluetooth address, in the "XX:XX:XX:XX:XX:XX" format.
     */
    BLUETOOTH,

    /**
     * USB. The ID is the USB vendor and product ID, in the "XXXX:XXXX" hex format.
     */
    USB,
}
