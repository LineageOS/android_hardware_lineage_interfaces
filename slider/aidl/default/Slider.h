/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/slider/BnSlider.h>
#include <android/binder_auto_utils.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#include "Source.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

class Slider : public BnSlider {
  public:
    Slider(std::unique_ptr<Source> source, const std::optional<SliderLocation>& location);
    ~Slider();

    bool start(bool verbose);

    ndk::ScopedAStatus getSliderInfo(SliderInfo* _aidl_return) override;
    ndk::ScopedAStatus getPosition(int32_t* _aidl_return) override;
    ndk::ScopedAStatus registerCallback(const std::shared_ptr<ISliderCallback>& callback) override;
    ndk::ScopedAStatus unregisterCallback(
            const std::shared_ptr<ISliderCallback>& callback) override;

  private:
    struct Registration {
        std::shared_ptr<ISliderCallback> callback;
        ndk::SpAIBinder binder;
        void* cookie;
    };

    bool startEventLoop();
    void eventLoop();
    void setPosition(int position);
    void notify(int position);
    void handleDeath(void* cookie);
    static void onCallbackDied(void* cookie);
    static void onCookieUnlinked(void* cookie);

    const std::unique_ptr<Source> mSource;
    const std::optional<SliderLocation> mLocation;
    int mCount = 0;
    std::atomic<int> mPosition{0};

    int mStopFd = -1;
    int mEpollFd = -1;
    std::thread mThread;

    // Serializes deliveries, so a registration cannot be overtaken by a
    // position change. Taken before mMutex wherever both are held.
    std::mutex mNotifyMutex;
    std::mutex mMutex;
    std::vector<Registration> mRegistrations;
    ndk::ScopedAIBinder_DeathRecipient mDeathRecipient;
};

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
