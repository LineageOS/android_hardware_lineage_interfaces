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

#ifndef __CRYPTO_HELPER_H__
#define __CRYPTO_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Human-readable strings for encryption modes, indexed by the encryption mode
#define NUM_ENCRYPTION_MODES 7
const char *const mode_strings[NUM_ENCRYPTION_MODES] = {
    "INVALID",     "AES-256-XTS", "AES-256-GCM", "AES-256-CBC",
    "AES-256-CTS", "AES-128-CBC", "AES-128-CTS"};

// Valid amounts of filename padding, indexed by the padding flag
#define NUM_PADDING_VALUES 4
const int padding_values[NUM_PADDING_VALUES] = {4, 8, 16, 32};

static uint8_t string_to_mode(const char *str);
static const char *mode_to_string(uint8_t mode);
static int string_to_padding_flag(const char *str);

#ifdef __cplusplus
}
#endif

#endif //__CRYPTO_HELPER_H__
