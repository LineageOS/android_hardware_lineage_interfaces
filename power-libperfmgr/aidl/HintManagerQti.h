/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cutils/properties.h>
#include <perfmgr/HintManager.h>

#include <optional>

namespace aidl {
namespace lineage {
namespace hardware {
namespace power {
namespace impl {
namespace qti {

using ::android::perfmgr::AdpfConfig;
using ::android::perfmgr::HintStats;
using ::android::perfmgr::HintStatus;
using ::android::perfmgr::NodeAction;
using ::android::perfmgr::OtherConfigs;

class PerfdBinding {
  public:
    struct PropVal {
        char value[PROP_VALUE_MAX];
    };

    PerfdBinding();
    ~PerfdBinding();

    int (*acquirePerfLock)(int handle, int duration, int list[], int num_args) = nullptr;
    int (*releasePerfLock)(int handle) = nullptr;
    int (*perfHint)(int hint_id, const char *pkg, int duration, int type) = nullptr;
    int (*perfHintAcqRel)(int handle, int hint_id, const char *pkg, int duration, int type,
                          int num_args, int list[]) = nullptr;
    PropVal (*perfGetProp)(const char *prop, const char *def_val) = nullptr;

  private:
    void *dl_handle_ = nullptr;
};

struct PerfHintAction {
    int id;
    int type = -1;
    std::optional<int> inactive_hint_id = std::nullopt;
    std::size_t node_index = 0;
};

struct Hint {
    Hint() = default;
    Hint(std::vector<PerfHintAction> actions,
         std::chrono::milliseconds timeout = std::chrono::milliseconds::zero())
        : perf_hint_actions(std::move(actions)), default_timeout(timeout) {}
    Hint(const Hint &obj)
        : node_actions(obj.node_actions),
          perf_hint_actions(obj.perf_hint_actions),
          default_timeout(obj.default_timeout),
          status(obj.status) {}

    std::vector<NodeAction> node_actions;
    std::vector<PerfHintAction> perf_hint_actions;
    std::chrono::milliseconds default_timeout = std::chrono::milliseconds::zero();
    mutable std::mutex hint_lock;
    std::shared_ptr<HintStatus> status GUARDED_BY(hint_lock);
};

class HintManagerQti {
  public:
    bool IsRunning() const;
    bool DoHint(const std::string &hint_type);
    bool DoHint(const std::string &hint_type, std::chrono::milliseconds timeout_ms_override);
    bool EndHint(const std::string &hint_type);
    bool IsHintSupported(const std::string &hint_type) const;
    bool IsHintEnabled(const std::string &hint_type) const;

    bool SetAdpfProfileFromDoHint(const std::string &profile_name);
    std::shared_ptr<AdpfConfig> GetAdpfProfileFromDoHint() const;

    bool SetAdpfProfile(const std::string &tag, const std::string &profile);

    typedef std::function<void(std::shared_ptr<AdpfConfig>)> AdpfCallback;
    void RegisterAdpfUpdateEvent(const std::string &tag, AdpfCallback *update_adpf_func);
    void UnregisterAdpfUpdateEvent(const std::string &tag, AdpfCallback *update_adpf_func);

    std::optional<std::string> gpu_sysfs_config_path() const;
    std::shared_ptr<AdpfConfig> GetAdpfProfile(const std::string &node_name = "OTHER") const;
    OtherConfigs GetOtherConfigs() const;
    bool IsAdpfSupported() const;
    bool IsAdpfProfileSupported(const std::string &name) const;
    std::vector<std::string> GetHints() const;
    HintStats GetHintStats(const std::string &hint_type) const;
    void DumpToFd(int fd);
    bool Start();
    static HintManagerQti *GetInstance();

  protected:
    static void Reload(bool start);
    static bool InitHintStatus(const std::unique_ptr<HintManagerQti> &hm);
    HintManagerQti(std::unordered_map<std::string, Hint> actions,
                   std::vector<std::unique_ptr<::android::perfmgr::Node>> nodes,
                   std::unique_ptr<PerfdBinding> binding);

    HintManagerQti(HintManagerQti const &) = delete;
    HintManagerQti &operator=(HintManagerQti const &) = delete;

    bool ValidateHint(const std::string &hint_type) const;
    void DoHintStatus(const std::string &hint_type, std::chrono::milliseconds timeout_ms);
    void EndHintStatus(const std::string &hint_type);
    bool DoHintInternal(const std::string &hint_type,
                        const std::vector<NodeAction> &node_actions,
                        std::chrono::milliseconds timeout_ms);

    std::unordered_map<std::string, Hint> actions_;
    std::unique_ptr<PerfdBinding> binding_;
    std::vector<std::unique_ptr<::android::perfmgr::Node>> nodes_;
    ::android::sp<::android::perfmgr::NodeLooperThread> nm_;

    static std::unique_ptr<HintManagerQti> sInstance;

};

}  // namespace qti
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace lineage
}  // namespace aidl
