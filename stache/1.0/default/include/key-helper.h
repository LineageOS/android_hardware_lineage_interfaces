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

#ifndef __KEY_HELPER_H__
#define __KEY_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "keyutils.h"
#include "sha512.h"
#include <stdio.h>

static void key_descriptor_to_hex(const uint8_t bytes[FS_KEY_DESCRIPTOR_SIZE],
                                  char hex[FS_KEY_DESCRIPTOR_HEX_SIZE]);
static int key_descriptor_to_bytes(const char *hex,
                                   uint8_t bytes[FS_KEY_DESCRIPTOR_SIZE]);
static int read_key(uint8_t key[FS_MAX_KEY_SIZE]);
static void compute_descriptor(const uint8_t key[FS_MAX_KEY_SIZE],
                               char descriptor[FS_KEY_DESCRIPTOR_HEX_SIZE]);
static int insert_logon_key(const uint8_t key_data[FS_MAX_KEY_SIZE],
                            const char descriptor[FS_KEY_DESCRIPTOR_HEX_SIZE],
                            const char *service_prefix);

#ifdef __cplusplus
}
#endif

#endif //__KEY_HELPER_H__
