/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "powerhal-libperfmgr"

#include "Headroom.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <sched.h>
#include <unistd.h>

#include <algorithm>
#include <cinttypes>
#include <sstream>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using ::android::base::ParseInt;
using ::android::base::ReadFileToString;
using ::android::base::StringPrintf;
using ::android::base::Trim;

namespace {

constexpr char kGpuDir[] = "/sys/class/kgsl/kgsl-3d0";

int64_t readInt(const std::string &path) {
    std::string s;
    int64_t v = 0;
    if (!ReadFileToString(path, &s) || !ParseInt(Trim(s), &v)) return -1;
    return v;
}

}  // namespace

Headroom &Headroom::getInstance() {
    static Headroom instance;
    return instance;
}

Headroom::Headroom() {
    mNumCpus = std::min<int>(sysconf(_SC_NPROCESSORS_CONF), kMaxCpus);
    for (int c = 0; c < mNumCpus; c++) {
        auto &info = mCpus[c];
        std::string related;
        if (!ReadFileToString(StringPrintf("/sys/devices/system/cpu/cpu%d/cpufreq/related_cpus", c),
                              &related)) {
            continue;
        }
        int first = c;
        std::istringstream in(related);
        in >> first;
        info.policy = first;
        info.topFreqKhz = readInt(
                StringPrintf("/sys/devices/system/cpu/cpufreq/policy%d/cpuinfo_max_freq", first));
        int64_t cap = readInt(StringPrintf("/sys/devices/system/cpu/cpu%d/cpu_capacity", c));
        info.capacity = cap > 0 ? cap : 1024;
        info.present = info.topFreqKhz > 0;
    }
    CpuTimes times[kMaxCpus];
    mCpuSupported = mNumCpus > 0 && mCpus[0].present && readCpuTimes(times);

    std::string freqs;
    if (ReadFileToString(StringPrintf("%s/gpu_available_frequencies", kGpuDir), &freqs)) {
        std::istringstream in(freqs);
        int64_t f;
        while (in >> f) mGpuTopFreqHz = std::max(mGpuTopFreqHz, f);
    }
    mGpuSupported = mGpuTopFreqHz > 0 && gpuNow() >= 0;
    LOG(INFO) << "Headroom: cpu " << (mCpuSupported ? "on" : "off") << " (" << mNumCpus
              << " CPUs), gpu " << (mGpuSupported ? "on" : "off");
}

bool Headroom::readCpuTimes(CpuTimes out[kMaxCpus]) {
    std::string stat;
    if (!ReadFileToString("/proc/stat", &stat)) return false;
    for (int c = 0; c < kMaxCpus; c++) out[c] = {};
    std::istringstream in(stat);
    std::string line;
    while (std::getline(in, line)) {
        int cpu;
        uint64_t user, nice, sys, idle, iowait, irq, softirq, steal;
        if (sscanf(line.c_str(), "cpu%d %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64
                                 " %" SCNu64 " %" SCNu64 " %" SCNu64,
                   &cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq, &steal) != 9) {
            continue;
        }
        if (cpu < 0 || cpu >= kMaxCpus) continue;
        out[cpu].busy = user + nice + sys + irq + softirq + steal;
        out[cpu].total = out[cpu].busy + idle + iowait;
        out[cpu].online = true;
    }
    return true;
}

float Headroom::gpuNow() {
    // gpubusy: "<busy> <total>" for the last kgsl stats period
    std::string s;
    if (!ReadFileToString(StringPrintf("%s/gpubusy", kGpuDir), &s)) return -1;
    int64_t busy = 0, total = 0;
    if (sscanf(s.c_str(), "%" SCNd64 " %" SCNd64, &busy, &total) != 2) return -1;
    int64_t cur = readInt(StringPrintf("%s/devfreq/cur_freq", kGpuDir));
    int64_t max = readInt(StringPrintf("%s/devfreq/max_freq", kGpuDir));
    if (cur <= 0 || max <= 0 || mGpuTopFreqHz <= 0) return -1;
    float load = total > 0 ? std::clamp(static_cast<float>(busy) / total, 0.f, 1.f) : 0.f;
    float used = load * cur / mGpuTopFreqHz;
    float cap = static_cast<float>(max) / mGpuTopFreqHz;
    return std::clamp(cap - used, 0.f, 1.f);
}

Headroom::Sample Headroom::takeSample(const CpuTimes prev[kMaxCpus], const CpuTimes cur[kMaxCpus]) {
    Sample s;
    s.time = std::chrono::steady_clock::now();
    int64_t curKhz[kMaxCpus] = {}, maxKhz[kMaxCpus] = {};
    for (int c = 0; c < mNumCpus; c++) {
        s.cpu[c] = 0;
        const auto &info = mCpus[c];
        if (!info.present || !cur[c].online || !prev[c].online) continue;
        int p = info.policy;
        if (curKhz[p] == 0) {
            curKhz[p] = readInt(
                    StringPrintf("/sys/devices/system/cpu/cpufreq/policy%d/scaling_cur_freq", p));
            maxKhz[p] = readInt(
                    StringPrintf("/sys/devices/system/cpu/cpufreq/policy%d/scaling_max_freq", p));
        }
        if (curKhz[p] <= 0 || maxKhz[p] <= 0) continue;
        uint64_t dt = cur[c].total - prev[c].total;
        uint64_t db = cur[c].busy - prev[c].busy;
        float load = dt > 0 ? std::clamp(static_cast<float>(db) / dt, 0.f, 1.f) : 0.f;
        float used = load * curKhz[p] / info.topFreqKhz;
        float cap = static_cast<float>(maxKhz[p]) / info.topFreqKhz;
        s.cpu[c] = std::clamp(cap - used, 0.f, 1.f);
    }
    for (int c = mNumCpus; c < kMaxCpus; c++) s.cpu[c] = 0;
    s.gpu = mGpuSupported ? gpuNow() : -1;
    return s;
}

void Headroom::ensureRunningLocked() {
    mLastRequest = std::chrono::steady_clock::now();
    if (mRunning) return;
    if (mThread.joinable()) mThread.join();
    mRunning = true;
    mSamples.clear();
    mThread = std::thread([this] { loop(); });
}

void Headroom::loop() {
    CpuTimes prev[kMaxCpus], cur[kMaxCpus];
    readCpuTimes(prev);
    std::unique_lock lock(mLock);
    while (true) {
        mWake.wait_for(lock, std::chrono::milliseconds(kSampleMillis));
        auto now = std::chrono::steady_clock::now();
        if (now - mLastRequest > kIdleStop) {
            mSamples.clear();
            mRunning = false;
            return;
        }
        lock.unlock();
        bool ok = readCpuTimes(cur);
        Sample s = takeSample(prev, cur);
        std::copy(cur, cur + kMaxCpus, prev);
        lock.lock();
        if (!ok) continue;
        mSamples.push_back(s);
        while (!mSamples.empty() &&
               now - mSamples.front().time > std::chrono::milliseconds(kMaxWindowMillis)) {
            mSamples.pop_front();
        }
        mWake.notify_all();
    }
}

std::vector<Headroom::Sample> Headroom::samplesInWindowLocked(int windowMillis) {
    ensureRunningLocked();
    // Lazy start: wait for the first sample (one sampling period)
    if (mSamples.empty()) {
        std::unique_lock lock(mLock, std::adopt_lock);
        mWake.wait_for(lock, std::chrono::milliseconds(kSampleMillis * 3),
                       [this] { return !mSamples.empty(); });
        lock.release();
    }
    auto from = std::chrono::steady_clock::now() - std::chrono::milliseconds(windowMillis);
    std::vector<Sample> out;
    for (const auto &s : mSamples) {
        if (s.time >= from) out.push_back(s);
    }
    if (out.empty() && !mSamples.empty()) out.push_back(mSamples.back());
    return out;
}

std::vector<int> Headroom::cpusFor(const std::vector<int32_t> &tids) {
    std::vector<int> cpus;
    cpu_set_t mask;
    CPU_ZERO(&mask);
    bool any = false;
    for (int32_t tid : tids) {
        cpu_set_t m;
        CPU_ZERO(&m);
        if (sched_getaffinity(tid, sizeof(m), &m) == 0) {
            CPU_OR(&mask, &mask, &m);
            any = true;
        }
    }
    for (int c = 0; c < mNumCpus; c++) {
        if (mCpus[c].present && (!any || CPU_ISSET(c, &mask))) cpus.push_back(c);
    }
    return cpus;
}

std::optional<float> Headroom::getCpuHeadroom(const std::vector<int32_t> &tids, int windowMillis,
                                              bool average) {
    if (!mCpuSupported) return std::nullopt;
    std::vector<int> cpus = cpusFor(tids);
    if (cpus.empty()) return std::nullopt;
    std::lock_guard lock(mLock);
    auto samples = samplesInWindowLocked(windowMillis);
    if (samples.empty()) return std::nullopt;
    int64_t capSum = 0;
    for (int c : cpus) capSum += mCpus[c].capacity;
    float minH = 1.f, sumH = 0.f;
    for (const auto &s : samples) {
        float h = 0.f;
        for (int c : cpus) h += s.cpu[c] * mCpus[c].capacity;
        h /= capSum;
        minH = std::min(minH, h);
        sumH += h;
    }
    return 100.f * (average ? sumH / samples.size() : minH);
}

std::optional<float> Headroom::getGpuHeadroom(int windowMillis, bool average) {
    if (!mGpuSupported) return std::nullopt;
    std::lock_guard lock(mLock);
    auto samples = samplesInWindowLocked(windowMillis);
    float minH = 1.f, sumH = 0.f;
    int n = 0;
    for (const auto &s : samples) {
        if (s.gpu < 0) continue;
        minH = std::min(minH, s.gpu);
        sumH += s.gpu;
        n++;
    }
    if (n == 0) return std::nullopt;
    return 100.f * (average ? sumH / n : minH);
}

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
