/*
** Copyright 2016, The CyanogenMod Project
**           2017-2018, The LineageOS Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "Utils.h"

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <string>

#include <cutils/sockets.h>

constexpr char LOCAL_STORAGE_PATH[] = "/data/vendor/display";
constexpr char LOCAL_MODE_ID[] = "livedisplay_mode";
constexpr char LOCAL_INITIAL_MODE_ID[] = "livedisplay_initial_mode";

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::NO_INIT;
using ::android::OK;
using ::android::status_t;

status_t Utils::readInt(const char* node, int32_t* value) {
    std::string buf;
    status_t ret = OK;
    std::ifstream fin(node);
    if (!fin.good()) {
        return errno;
    }
    fin >> *value;
    if (fin.fail()) {
        ret = errno;
    }
    fin.close();
    return ret;
}

status_t Utils::writeInt(const char* node, int32_t value) {
    status_t ret = OK;
    std::ofstream fout(node);
    if (!fout.good()) {
        return errno;
    }
    fout << value << std::endl;
    if (fout.fail()) {
        ret = errno;
    }
    fout.close();
    return ret;
}

status_t Utils::readLocalModeId(int32_t* id) {
    char buf[PATH_MAX];
    sprintf(buf, "%s/%s", LOCAL_STORAGE_PATH, LOCAL_MODE_ID);
    return readInt(buf, id);
}

status_t Utils::writeLocalModeId(int32_t id) {
    char buf[PATH_MAX];
    sprintf(buf, "%s/%s", LOCAL_STORAGE_PATH, LOCAL_MODE_ID);
    return writeInt(buf, id);
}

status_t Utils::readInitialModeId(int32_t* id) {
    char buf[PATH_MAX];
    sprintf(buf, "%s/%s", LOCAL_STORAGE_PATH, LOCAL_INITIAL_MODE_ID);
    return readInt(buf, id);
}

status_t Utils::writeInitialModeId(int32_t id) {
    char buf[PATH_MAX];
    sprintf(buf, "%s/%s", LOCAL_STORAGE_PATH, LOCAL_INITIAL_MODE_ID);
    return writeInt(buf, id);
}

status_t Utils::sendDPPSCommand(char* buf, size_t len) {
    status_t rc = OK;
    int sock = socket_local_client("pps", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
    if (sock < 0) {
        return sock;
    }

    if (write(sock, buf, strlen(buf) + 1) > 0) {
        memset(buf, 0, len);
        ssize_t ret;
        while ((ret = read(sock, buf, len)) > 0) {
            if ((size_t)ret == len) {
                break;
            }
            len -= ret;
            buf += ret;

            struct pollfd p = {.fd = sock, .events = POLLIN, .revents = 0};

            ret = poll(&p, 1, 20);
            if ((ret <= 0) || !(p.revents & POLLIN)) {
                break;
            }
        }
    } else {
        rc = NO_INIT;
    }
    close(sock);
    return rc;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
