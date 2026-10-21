/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <linux/input.h>

#include <functional>
#include <string>
#include <vector>

#include "Source.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

// What every source that watches an input device owns, whether the position
// is in the events, in a file beside them or in the device's own state.
class InputSource : public Source {
  public:
    ~InputSource() override;

    int descriptor() const final { return mFd; }

  protected:
    explicit InputSource(const std::vector<std::string>& inputNames);

    // Takes everything the device has queued, handing each event to handle().
    // Returns true where the kernel overflowed the queue, after which the
    // events left describe a slider that has already moved on, so they are
    // dropped and the caller reads the hardware instead.
    bool drain(const std::function<void(const struct input_event&)>& handle);

    const std::vector<std::string> mInputNames;
    int mFd = -1;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
