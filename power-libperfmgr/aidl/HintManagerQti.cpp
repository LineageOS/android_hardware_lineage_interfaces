/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define ATRACE_TAG (ATRACE_TAG_POWER | ATRACE_TAG_HAL)
#define LOG_TAG "libperfmgr"

#include "HintManagerQti.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <dlfcn.h>
#include <perfmgr/PropertyNode.h>
#include <unistd.h>
#include <utils/Trace.h>

#include <algorithm>
#include <cinttypes>
#include <cstdlib>
#include <limits>

#include "QtiHintNode.h"

using namespace std::chrono_literals;

namespace aidl {
namespace lineage {
namespace hardware {
namespace power {
namespace impl {
namespace qti {

using ::android::perfmgr::RequestGroup;

namespace {
constexpr auto kMilliSecondZero = 0ms;
constexpr auto kVendorDefaultTimeout = -1ms;
const auto kTimePointMax = std::chrono::steady_clock::time_point::max();

constexpr char kPowerHalStateProp[] = "vendor.powerhal.state";
constexpr char kExpensiveRenderingProp[] = "vendor.perf.expensive_rendering";
constexpr char kDisabledPropValue[] = "0";

constexpr int kVendorHintDisplayOff = 0x00001040;
constexpr int kVendorHintDisplayOn = 0x00001041;
constexpr int kVendorHintScrollBoost = 0x00001080;
constexpr int kVendorHintFirstLaunchBoost = 0x00001081;

constexpr int kPowerHintAospDelta = 0x00001200;
constexpr int kPerfHintSustainedPerformance = kPowerHintAospDelta + 0x00000006;
constexpr int kPerfHintExpensiveRendering = 0x000010A5;

enum Launch {
    kLaunchBoostV1 = 1,
    kLaunchBoostV2 = 2,
    kLaunchBoostV3 = 3,
    kLaunchBoostGame = 4,
    kLaunchReserved1 = 5,
    kLaunchReserved2 = 6,
    kLaunchReserved3 = 7,
    kLaunchReserved4 = 8,
    kLaunchReserved5 = 9,
    kLaunchActivityBoost = 10,
    kLaunchTypeServiceStart = 100,
    kLaunchTypeStartProc = 101,
    kLaunchTypeStartAppFromBg = 102,
    kLaunchTypeAttachApplication = 103,
};

enum Scroll {
    kScrollVertical = 1,
    kScrollHorizontal = 2,
    kScrollPanelView = 3,
    kScrollPrefiling = 4,
};

std::chrono::milliseconds ToStatusTimeout(std::chrono::milliseconds timeout_ms) {
    return timeout_ms == kVendorDefaultTimeout ? kMilliSecondZero : timeout_ms;
}

}  // namespace

HintManagerQti::HintManagerQti(std::unordered_map<std::string, Hint> actions,
                               std::vector<std::unique_ptr<::android::perfmgr::Node>> nodes,
                               std::unique_ptr<PerfdBinding> binding)
    : actions_(std::move(actions)), binding_(std::move(binding)), nodes_(std::move(nodes)) {}

bool HintManagerQti::ValidateHint(const std::string &hint_type) const {
    if (!IsRunning()) {
        LOG(ERROR) << "NodeLooperThread not present";
        return false;
    }
    return IsHintSupported(hint_type);
}

bool HintManagerQti::IsHintSupported(const std::string &hint_type) const {
    if (actions_.find(hint_type) == actions_.end()) {
        LOG(DEBUG) << "Hint type not present in actions: " << hint_type;
        return false;
    }
    return true;
}

bool HintManagerQti::IsHintEnabled(const std::string &hint_type) const {
    if (hint_type == "EXPENSIVE_RENDERING") {
        if (binding_ == nullptr || binding_->perfGetProp == nullptr) {
            return false;
        }
        const auto prop = binding_->perfGetProp(kExpensiveRenderingProp, kDisabledPropValue);
        if (std::atoi(prop.value) != 1) {
            return false;
        }
    }

    return true;
}

bool HintManagerQti::InitHintStatus(const std::unique_ptr<HintManagerQti> &hm) {
    if (hm.get() == nullptr) {
        return false;
    }
    for (auto &[_, hint] : hm->actions_) {
        std::lock_guard<std::mutex> lock(hint.hint_lock);
        hint.status = std::make_shared<HintStatus>(ToStatusTimeout(hint.default_timeout));
    }
    return true;
}

void HintManagerQti::DoHintStatus(const std::string &hint_type,
                                  std::chrono::milliseconds timeout_ms) {
    std::lock_guard<std::mutex> lock(actions_.at(hint_type).hint_lock);
    actions_.at(hint_type).status->stats.count.fetch_add(1);
    auto now = std::chrono::steady_clock::now();
    ATRACE_INT(("H:" + hint_type).c_str(), (timeout_ms == kMilliSecondZero)
                                                   ? std::numeric_limits<int>::max()
                                                   : timeout_ms.count());
    ATRACE_NAME(("H:" + hint_type + ":" +
                 std::to_string((timeout_ms == kMilliSecondZero) ? std::numeric_limits<int>::max()
                                                                 : timeout_ms.count()))
                        .c_str());
    if (now > actions_.at(hint_type).status->end_time) {
        actions_.at(hint_type).status->stats.duration_ms.fetch_add(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        actions_.at(hint_type).status->end_time -
                        actions_.at(hint_type).status->start_time)
                        .count());
        actions_.at(hint_type).status->start_time = now;
    }
    actions_.at(hint_type).status->end_time =
            (timeout_ms == kMilliSecondZero) ? kTimePointMax : now + timeout_ms;
}

void HintManagerQti::EndHintStatus(const std::string &hint_type) {
    std::lock_guard<std::mutex> lock(actions_.at(hint_type).hint_lock);
    auto now = std::chrono::steady_clock::now();
    ATRACE_INT(("H:" + hint_type).c_str(), 0);
    ATRACE_NAME(("H:" + hint_type + ":0").c_str());
    if (now < actions_.at(hint_type).status->end_time) {
        actions_.at(hint_type).status->stats.duration_ms.fetch_add(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        now - actions_.at(hint_type).status->start_time)
                        .count());
        actions_.at(hint_type).status->end_time = now;
    }
}

bool HintManagerQti::DoHintInternal(const std::string &hint_type,
                                    const std::vector<NodeAction> &node_actions,
                                    std::chrono::milliseconds timeout_ms) {
    if (!IsHintEnabled(hint_type)) {
        return false;
    }

    if (!nm_->Request(node_actions, hint_type)) {
        return false;
    }

    DoHintStatus(hint_type, ToStatusTimeout(timeout_ms));
    return true;
}

bool HintManagerQti::DoHint(const std::string &hint_type) {
    LOG(VERBOSE) << "Do Powerhint: " << hint_type;
    if (!ValidateHint(hint_type)) {
        return false;
    }

    const auto &hint = actions_.at(hint_type);
    return DoHintInternal(hint_type, hint.node_actions, hint.default_timeout);
}

bool HintManagerQti::DoHint(const std::string &hint_type,
                            std::chrono::milliseconds timeout_ms_override) {
    LOG(VERBOSE) << "Do Powerhint: " << hint_type << " for " << timeout_ms_override.count() << "ms";
    if (!ValidateHint(hint_type)) {
        return false;
    }

    const auto &hint = actions_.at(hint_type);
    std::vector<NodeAction> actions_override = hint.node_actions;
    for (auto &action : actions_override) {
        bool is_qti_hint_node = false;
        for (const auto &perf_hint_action : hint.perf_hint_actions) {
            if (perf_hint_action.node_index == action.node_index) {
                action = QtiHintNode::GetNodeAction(action.node_index, timeout_ms_override);
                is_qti_hint_node = true;
                break;
            }
        }
        if (!is_qti_hint_node) {
            action.timeout_ms = timeout_ms_override;
        }
    }

    return DoHintInternal(hint_type, actions_override, timeout_ms_override);
}

bool HintManagerQti::EndHint(const std::string &hint_type) {
    LOG(VERBOSE) << "End Powerhint: " << hint_type;
    if (!ValidateHint(hint_type) || !nm_->Cancel(actions_.at(hint_type).node_actions, hint_type)) {
        return false;
    }

    EndHintStatus(hint_type);
    return true;
}

bool HintManagerQti::IsRunning() const {
    return nm_.get() != nullptr && nm_->isRunning();
}

std::vector<std::string> HintManagerQti::GetHints() const {
    std::vector<std::string> hints;
    for (const auto &action : actions_) {
        hints.push_back(action.first);
    }
    return hints;
}

HintStats HintManagerQti::GetHintStats(const std::string &hint_type) const {
    HintStats hint_stats;
    if (ValidateHint(hint_type)) {
        std::lock_guard<std::mutex> lock(actions_.at(hint_type).hint_lock);
        hint_stats.count =
                actions_.at(hint_type).status->stats.count.load(std::memory_order_relaxed);
        hint_stats.duration_ms =
                actions_.at(hint_type).status->stats.duration_ms.load(std::memory_order_relaxed);
    }
    return hint_stats;
}

void HintManagerQti::DumpToFd(int fd) {
    std::string header("========== Begin qti perfmgr nodes ==========\n");
    if (!android::base::WriteStringToFd(header, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
    if (nm_.get() != nullptr) {
        nm_->DumpToFd(fd);
    }
    std::string footer("==========  End qti perfmgr nodes  ==========\n");
    if (!android::base::WriteStringToFd(footer, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }

    std::vector<std::string> keys(GetHints());
    std::sort(keys.begin(), keys.end());
    header = "========== Begin qti perfmgr stats ==========\n"
             "Hint Name\t"
             "Counts\t"
             "Duration\n";
    if (!android::base::WriteStringToFd(header, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
    std::string hint_stats_string;
    for (const auto &ordered_key : keys) {
        HintStats hint_stats(GetHintStats(ordered_key));
        hint_stats_string +=
                android::base::StringPrintf("%s\t%" PRIu32 "\t%" PRIu64 "\n", ordered_key.c_str(),
                                            hint_stats.count, hint_stats.duration_ms);
    }
    if (!android::base::WriteStringToFd(hint_stats_string, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
    footer = "==========  End qti perfmgr stats  ==========\n";
    if (!android::base::WriteStringToFd(footer, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }

    header = "========== IsAdpfSupported: No ===========\n";
    if (!android::base::WriteStringToFd(header, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
    fsync(fd);
}

bool HintManagerQti::Start() {
    if (nm_.get() == nullptr) {
        nm_ = new ::android::perfmgr::NodeLooperThread(std::move(nodes_));
    }

    return nm_->Start();
}

std::unique_ptr<HintManagerQti> HintManagerQti::sInstance = nullptr;

void HintManagerQti::Reload(bool start) {
    auto binding = std::make_unique<PerfdBinding>();

    std::unordered_map<std::string, Hint> actions;
    std::vector<std::unique_ptr<::android::perfmgr::Node>> nodes;
    std::size_t node_index = 0;

    auto add_property_node = [&](const std::string &hint_name, Hint *hint, const char *property,
                                 const char *value, const char *default_value) {
        const std::size_t action_index = hint->node_actions.size();
        nodes.push_back(std::make_unique<::android::perfmgr::PropertyNode>(
                android::base::StringPrintf("%s#%zu", hint_name.c_str(), action_index),
                std::vector<std::string>{property},
                std::vector<RequestGroup>{RequestGroup(value), RequestGroup(default_value)}, 1,
                false));
        hint->node_actions.emplace_back(node_index, 0, hint->default_timeout);
        ++node_index;
    };
    auto add_hint = [&](const std::string &hint_name, Hint hint) {
        auto [it, _] = actions.emplace(hint_name, std::move(hint));
        for (auto &hint_action : it->second.perf_hint_actions) {
            const std::size_t action_index = it->second.node_actions.size();
            nodes.push_back(std::make_unique<QtiHintNode>(
                    android::base::StringPrintf("%s#%zu", it->first.c_str(), action_index),
                    hint_action.id, hint_action.type, *binding, hint_action.inactive_hint_id));
            hint_action.node_index = node_index;
            it->second.node_actions.push_back(
                    QtiHintNode::GetNodeAction(node_index, it->second.default_timeout));
            ++node_index;
        }
        return &it->second;
    };

    add_hint("LAUNCH", Hint{{{kVendorHintFirstLaunchBoost, kLaunchBoostV1},
                             {kVendorHintFirstLaunchBoost, kLaunchBoostV2},
                             {kVendorHintFirstLaunchBoost, kLaunchBoostV3}},
                            kVendorDefaultTimeout});
    add_hint("GAME_LOADING", Hint{{{kVendorHintFirstLaunchBoost, kLaunchBoostV1},
                                   {kVendorHintFirstLaunchBoost, kLaunchBoostV2},
                                   {kVendorHintFirstLaunchBoost, kLaunchBoostGame}},
                                  kVendorDefaultTimeout});
    add_hint("INTERACTION", Hint{{{kVendorHintScrollBoost, kScrollHorizontal}}, kMilliSecondZero});
    add_hint("INTERACTIVE",
             Hint{{{kVendorHintDisplayOn, 0, kVendorHintDisplayOff}}, kMilliSecondZero});
    add_hint("EXPENSIVE_RENDERING", Hint{{{kPerfHintExpensiveRendering}}, kMilliSecondZero});
    Hint *sustained_performance = add_hint(
            "SUSTAINED_PERFORMANCE", Hint{{{kPerfHintSustainedPerformance}}, kMilliSecondZero});
    add_property_node("SUSTAINED_PERFORMANCE", sustained_performance, kPowerHalStateProp,
                      "SUSTAINED_PERFORMANCE", "");
    add_hint("FIXED_PERFORMANCE", Hint{{{kPerfHintSustainedPerformance}}, kMilliSecondZero});

    auto hint_manager = std::unique_ptr<HintManagerQti>(
            new HintManagerQti(std::move(actions), std::move(nodes), std::move(binding)));
    if (!InitHintStatus(hint_manager)) {
        LOG(FATAL) << "Failed to initialize HintManagerQti hint status";
    }

    sInstance = std::move(hint_manager);
    if (start) {
        sInstance->Start();
    }
}

HintManagerQti *HintManagerQti::GetInstance() {
    if (sInstance == nullptr) {
        HintManagerQti::Reload(false);
    }
    return sInstance.get();
}

std::shared_ptr<AdpfConfig> HintManagerQti::GetAdpfProfileFromDoHint() const {
    return nullptr;
}

bool HintManagerQti::SetAdpfProfileFromDoHint(const std::string &profile_name) {
    (void)profile_name;
    return false;
}

bool HintManagerQti::IsAdpfSupported() const {
    return false;
}

std::shared_ptr<AdpfConfig> HintManagerQti::GetAdpfProfile(const std::string &tag) const {
    (void)tag;
    return nullptr;
}

OtherConfigs HintManagerQti::GetOtherConfigs() const {
    return {};
}

bool HintManagerQti::SetAdpfProfile(const std::string &tag, const std::string &profile) {
    (void)tag;
    (void)profile;
    return false;
}

bool HintManagerQti::IsAdpfProfileSupported(const std::string &profile_name) const {
    (void)profile_name;
    return false;
}

void HintManagerQti::RegisterAdpfUpdateEvent(const std::string &tag,
                                             AdpfCallback *update_adpf_func) {
    (void)tag;
    (void)update_adpf_func;
}

void HintManagerQti::UnregisterAdpfUpdateEvent(const std::string &tag,
                                               AdpfCallback *update_adpf_func) {
    (void)tag;
    (void)update_adpf_func;
}

std::optional<std::string> HintManagerQti::gpu_sysfs_config_path() const {
    return {};
}

#define BIND_PERF_METHOD(func, name)                                  \
    func = reinterpret_cast<decltype(func)>(dlsym(dl_handle_, name)); \
    LOG_IF(FATAL, func == nullptr) << "Failed to find required symbol " << name;

#define BIND_OPTIONAL_PERF_METHOD(func, name)                         \
    func = reinterpret_cast<decltype(func)>(dlsym(dl_handle_, name)); \
    LOG_IF(INFO, func == nullptr) << "Optional symbol not found: " << name;

PerfdBinding::PerfdBinding() {
    constexpr auto kPerfModule = "libqti-perfd-client.so";
    dl_handle_ = dlopen(kPerfModule, RTLD_NOW | RTLD_NODELETE);
    if (dl_handle_ == nullptr) {
        LOG(FATAL) << "Failed to dlopen " << kPerfModule;
    }

    BIND_PERF_METHOD(acquirePerfLock, "perf_lock_acq");
    BIND_PERF_METHOD(releasePerfLock, "perf_lock_rel");
    BIND_PERF_METHOD(perfHint, "perf_hint");
    BIND_OPTIONAL_PERF_METHOD(perfHintAcqRel, "perf_hint_acq_rel");
    BIND_OPTIONAL_PERF_METHOD(perfGetProp, "perf_get_prop");
}

PerfdBinding::~PerfdBinding() {
    acquirePerfLock = nullptr;
    releasePerfLock = nullptr;
    perfHint = nullptr;
    perfHintAcqRel = nullptr;
    perfGetProp = nullptr;
    if (dl_handle_) {
        dlclose(dl_handle_);
        dl_handle_ = nullptr;
    }
}

}  // namespace qti
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace lineage
}  // namespace aidl
