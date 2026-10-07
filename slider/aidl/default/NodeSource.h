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

// A file holding the position, beside an input device that reports when it
// changes without saying what it changed to.
class NodeSource : public Source {
  public:
    NodeSource(const std::vector<std::string>& inputNames, const std::string& node,
               const std::vector<int>& values);
    ~NodeSource() override;

    bool open(bool verbose) override;
    int count() const override { return static_cast<int>(mValues.size()); }
    int descriptor() const override { return mFd; }
    std::optional<int> position() override;
    void read(const std::function<void(int)>& moved) override;

  private:
    const std::vector<std::string> mInputNames;
    const std::string mNode;
    const std::vector<int> mValues;
    int mFd = -1;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
