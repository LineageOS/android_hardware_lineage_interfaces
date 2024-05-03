# vendor.lineage.accessories

## `IAccessories`

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

### Battery status

An accessory may be able to provide battery information, although it should be implemented only if
it won't be available through the AOSP provided system APIs, like Bluetooth devices.

It's valid to query the battery status of an accessory only when:

- The accessory reports support for battery reporting through the field
  `AccessoryInfo.supportsBatteryQuerying`
- The accessory is not in the `DISCONNECTED` state

## Support library

The static support library `vendor.lineage.accessories-support-ndk` contains a few classes that will
help with the implementation of the `IAccessories` interface and its accessories.

The fastest way to include it is by adding the following line to your `cc_binary` rule:

```
cc_binary {
    ...
    defaults: ["vendor.lineage.accessories-support-ndk-defaults"],
    ...
}
```

This will both add the support lib as a static library and also add the NDK library on the latest
interface version as a shared one (make sure you don't already include a different version of the
NDK library!).
