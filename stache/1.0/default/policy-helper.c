/*
 * Copyright (C) 2017 Google Inc.
 * Author: Joe Richey (joerichey@google.com)
 *
 * Modifications Copyright (C) 2018 The LineageOS Project
 * Modifications Author: Rashed Abdel-Tawab (rashed@linux.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <sys/ioctl.h>

#include "linux/fs.h"
#include "policy-helper.h"

static int get_policy(const char *path, struct fscrypt_policy *policy) {
    // We can query the policy for a directory or a file in that directory.
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return -1;

    int ret = ioctl(fd, FS_IOC_GET_ENCRYPTION_POLICY, policy);
    close(fd);

    if (ret < 0) {
        // Kernels prior to v4.11 returned ENOENT if the file did not have an
        // encryption policy, newer kernels properly return ENODATA. This lets us
        // print the right error in policy_error regardless of kernel version.
        if (errno == ENOENT)
            errno = ENODATA;
    }
    return ret;
}

static int set_policy(const char *path, const struct fscrypt_policy *policy) {
    // Policies can only be set on directories
    int fd = open(path, O_RDONLY | O_DIRECTORY);
    if (fd < 0)
        return -1;

    int ret = ioctl(fd, FS_IOC_SET_ENCRYPTION_POLICY, policy);
    close(fd);

    return ret;
}
