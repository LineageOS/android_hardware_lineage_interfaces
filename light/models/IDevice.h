/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <models/IDumpable.h>
#include <models/State.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/**
 * Interface for light devices.
 */
class IDevice : public IDumpable {
  public:
    virtual ~IDevice() = default;

    /**
     * Return whether this device exists and is functioning correctly.
     */
    virtual bool isOk() const = 0;

    /**
     * Set the state of this device.
     *
     * @param state The state to set
     * @return bool true if the state was set successfully, false otherwise
     */
    virtual bool setState(const State& state) = 0;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
