/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "InputDevice.h"

#include <android-base/logging.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>

namespace aidl {
namespace vendor {
namespace lineage {
namespace slider {

namespace {
constexpr char kInputDir[] = "/dev/input";
}  // namespace

int openInputDevice(const std::vector<std::string>& names, bool verbose,
                    const std::function<bool(int fd, const std::string& name)>& accept) {
    DIR* dir = opendir(kInputDir);
    if (dir == nullptr) {
        PLOG(ERROR) << "Failed to open " << kInputDir;
        return -1;
    }

    int opened = -1;
    struct dirent* entry;
    while (opened < 0 && (entry = readdir(dir)) != nullptr) {
        if (strncmp(entry->d_name, "event", 5) != 0) {
            continue;
        }

        std::string path = std::string(kInputDir) + "/" + entry->d_name;
        int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
        if (fd < 0) {
            continue;
        }

        char name[256] = {};
        if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), name) < 0 ||
            std::find(names.begin(), names.end(), name) == names.end()) {
            close(fd);
            continue;
        }

        if (!accept(fd, name)) {
            close(fd);
            continue;
        }

        LOG(INFO) << "Opened " << name << " (" << path << ")";
        opened = fd;
    }

    closedir(dir);

    if (opened < 0 && verbose) {
        LOG(ERROR) << "No matching slider input device found";
    }

    return opened;
}

}  // namespace slider
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
