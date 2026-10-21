/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "InputSource.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

// One switch code per position, set for as long as the slider rests in it,
// or a single code whose two states are the two positions.
// A switch carries a state rather than a keystroke, so the codes travel the
// input stack without anything acting on them.
class SwitchSource : public InputSource {
  public:
    SwitchSource(const std::vector<std::string>& inputNames, const std::vector<int>& codes);

    bool open(bool verbose) override;
    int count() const override { return mCount; }
    std::optional<int> position() override;
    void read(const std::function<void(int)>& moved) override;

  private:
    std::optional<int> positionOf(const std::vector<bool>& set) const;

    const std::vector<int> mCodes;
    const int mCount;
    std::vector<bool> mSet;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
