/*
** Copyright 2016, The CyanogenMod Project
**           2017, The LineageOS Project
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/stat.h>

#include <cutils/sockets.h>

#include "Utils.h"

#define LOCAL_STORAGE_PATH "/data/vendor/display"
#define LOCAL_MODE_ID "livedisplay_mode"
#define LOCAL_INITIAL_MODE_ID "livedisplay_initial_mode"

namespace android {

status_t Utils::exists(const char* node) {

    struct stat sbuf;
    return stat(node, &sbuf);
}

status_t Utils::readInt(const char* node, int32_t* value) {
    char buf[32];
    status_t ret = OK;

    FILE *fp = fopen(node, "r");
    if (!fp) {
        return errno;
    }
    if (fgets(buf, sizeof(buf) - 1, fp)) {
        *value = atoi(buf);
    } else {
        ret = errno;
    }
    fclose(fp);
    return ret;
}

status_t Utils::writeInt(const char* node, int32_t value) {
    char buf[32];
    status_t ret = OK;

    FILE *fp = fopen(node, "w");
    if (!fp) {
        return errno;
    }
    snprintf(buf, sizeof(buf), "%d\n", value);

    if (fputs(buf, fp) < 0) {
        ret = errno;
    }
    fclose(fp);
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
    } else {
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
    }
    return rc;
}

};
