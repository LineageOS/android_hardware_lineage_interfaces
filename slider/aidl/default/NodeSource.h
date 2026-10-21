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

// A file holding the position, beside an input device that reports when it
// changes without saying what it changed to.
class NodeSource : public InputSource {
  public:
    NodeSource(const std::vector<std::string>& inputNames, const std::string& node,
               const std::vector<int>& values);

    bool open(bool verbose) override;
    int count() const override { return static_cast<int>(mValues.size()); }
    std::optional<int> position() override;
    void read(const std::function<void(int)>& moved) override;

  private:
    const std::string mNode;
    const std::vector<int> mValues;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
