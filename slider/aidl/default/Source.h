/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>
#include <optional>

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

// Where a slider's position comes from. What the interface reports, and who
// it reports to, is the same whatever answers these.
class Source {
  public:
    virtual ~Source() = default;

    // Opens the hardware, saying why it cannot when verbose.
    virtual bool open(bool verbose) = 0;

    // How many positions the hardware has, once it is open.
    virtual int count() const = 0;

    // The descriptor that becomes readable when the slider moves.
    virtual int descriptor() const = 0;

    // Where the slider rests, asked of the hardware rather than remembered.
    virtual std::optional<int> position() = 0;

    // Called while the descriptor is readable, reporting every position the
    // slider was seen in, since one read can hold several moves.
    virtual void read(const std::function<void(int)>& moved) = 0;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
