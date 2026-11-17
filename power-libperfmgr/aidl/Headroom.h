/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

// CPU and GPU headroom (IPower getCpuHeadroom / getGpuHeadroom, SystemHealthManager) from
// /proc/stat, cpufreq and kgsl. A sampler thread runs only while clients ask: it starts on the
// first request and stops kIdleStop after the last one.
//
// Headroom of a CPU = capacity the frequency cap still allows minus the capacity in use
// (busy share x current frequency), both relative to the CPU's top frequency, weighted by
// cpu_capacity when several CPUs are asked for. GPU likewise from the kgsl busy share and the
// devfreq current/max frequency. Values are percentages in [0, 100].
class Headroom {
  public:
    static constexpr int kSampleMillis = 50;
    static constexpr int kMinWindowMillis = 50;
    static constexpr int kMaxWindowMillis = 10000;
    static constexpr int kMaxTids = 5;

    static Headroom &getInstance();

    bool cpuSupported() const { return mCpuSupported; }
    bool gpuSupported() const { return mGpuSupported; }

    // tids empty = all CPUs, else the CPUs those threads may run on.
    std::optional<float> getCpuHeadroom(const std::vector<int32_t> &tids, int windowMillis,
                                        bool average);
    std::optional<float> getGpuHeadroom(int windowMillis, bool average);

  private:
    static constexpr int kMaxCpus = 16;
    static constexpr std::chrono::seconds kIdleStop{15};

    struct CpuInfo {
        bool present = false;
        int policy = -1;          // first CPU of its cpufreq policy
        int64_t topFreqKhz = 0;   // cpuinfo_max_freq
        int64_t capacity = 1024;  // cpu_capacity
    };
    struct Sample {
        std::chrono::steady_clock::time_point time;
        float cpu[kMaxCpus];  // per-CPU headroom fraction [0, 1]; offline = 0
        float gpu;            // [0, 1], or -1 when not available
    };
    struct CpuTimes {
        uint64_t busy = 0;
        uint64_t total = 0;
        bool online = false;
    };

    Headroom();
    void ensureRunningLocked();
    void loop();
    bool readCpuTimes(CpuTimes out[kMaxCpus]);
    Sample takeSample(const CpuTimes prev[kMaxCpus], const CpuTimes cur[kMaxCpus]);
    float gpuNow();
    std::vector<int> cpusFor(const std::vector<int32_t> &tids);
    std::vector<Sample> samplesInWindowLocked(int windowMillis);

    int mNumCpus = 0;
    CpuInfo mCpus[kMaxCpus];
    bool mCpuSupported = false;
    bool mGpuSupported = false;
    int64_t mGpuTopFreqHz = 0;

    std::mutex mLock;
    std::condition_variable mWake;
    std::deque<Sample> mSamples;
    std::chrono::steady_clock::time_point mLastRequest;
    bool mRunning = false;
    std::thread mThread;
};

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
