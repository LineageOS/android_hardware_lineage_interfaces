# vendor.lineage.input

## Accessories

### `IAccessories`

`IAccessories` is an interface that allows managing accessories.

This interface should more or less mimic the behavior of Bluetooth devices, but it's not limited to
Bluetooth accessories.

For some accessories it might be desirable to disable and enable accessories, for example, a
keyboard accessory that is connected but disabled.

It should also support querying the battery status of the accessory, for example with an active pen
that is charged by the device.

### Persistent accessories

Since there isn't a "forget accessory" logic with this interface, non-core accessories should be
removed from the UI when they are disconnected.

As an example, accessories like game controllers should only be displayed in the UI when they are
connected since the count of connected game controllers is dynamic and they're independent from
the device.

Some type of accessories though are persistent, meaning that they should be displayed in the UI even
when they are not connected. For example, a detachable keyboard should be displayed in the UI even
when it is not connected, since it's a core part of the device, and it most likely have a dedicated
port.

This is also applicable to active pens that are charged by the device, but are not always connected
(as in active).

### Accessories status

Depending on the type, an accessory can have multiple states. The following states are supported:

- `AccessoryStatus.DISCONNECTED`: The accessory is not connected, thus cannot be enabled. If this
  isn't a persistent accessory, it should be removed from the UI.
- `AccessoryStatus.CHARGING`: The accessory, while technically connected to the device, is not in a
  state where it can be used. Instead it is currently charging. This is the case for an active pen
  physically connected to the device. The accessory should be able to provide battery information.
- `AccessoryStatus.DISABLED`: The device is connected but disabled.
- `AccessoryStatus.ENABLED`: The device is connected and enabled.

The state can only be switched between `ENABLED` and `DISABLED`, and the accessory should be able to
provide the current state.
