/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnDisplayModes.h>
#include <android-base/macros.h>

#include <functional>

#include "SDMController.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace sdm {

class DisplayModes : public BnDisplayModes {
  public:
    explicit DisplayModes(std::shared_ptr<SDMController> controller);

    using DisplayModeSetCallback = std::function<void()>;
    virtual void registerDisplayModeSetCallback(DisplayModeSetCallback callback);

    // Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayModes follow.
    ndk::ScopedAStatus getDisplayModes(std::vector<DisplayMode>* _aidl_return) override;
    ndk::ScopedAStatus getCurrentDisplayMode(DisplayMode* _aidl_return) override;
    ndk::ScopedAStatus getDefaultDisplayMode(DisplayMode* _aidl_return) override;
    ndk::ScopedAStatus setDisplayMode(int32_t modeID, bool makeDefault,
                                      bool* _aidl_return) override;

  protected:
    std::shared_ptr<SDMController> controller_;
    DisplayModeSetCallback on_display_mode_set_;

    virtual bool isReady();
    virtual std::vector<DisplayMode> getDisplayModesInternal();
    virtual DisplayMode getDisplayModeById(int32_t id);
    virtual DisplayMode getCurrentDisplayModeInternal();
    virtual DisplayMode getDefaultDisplayModeInternal();

  private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(DisplayModes);
};

}  // namespace sdm
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
