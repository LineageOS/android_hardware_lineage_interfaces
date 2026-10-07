/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "Source.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

// One EV_ABS axis on an input device, where the axis value is the position
// and the range it advertises is how many there are.
class AxisSource : public Source {
  public:
    AxisSource(const std::vector<std::string>& inputNames, int absCode);
    ~AxisSource() override;

    bool open(bool verbose) override;
    int count() const override { return mCount; }
    int descriptor() const override { return mFd; }
    std::optional<int> position() override;
    void read(const std::function<void(int)>& moved) override;

  private:
    const std::vector<std::string> mInputNames;
    const int mAbsCode;
    int mFd = -1;
    int mCount = 0;
    int mMinimum = 0;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
