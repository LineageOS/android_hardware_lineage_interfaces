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

#include "crypto-helper.h"

/*
 * Converts str to an encryption mode. Returns 0 (FS_ENCRYPTION_MODE_INVALID) if
 * the string does not correspond to an encryption mode.
 */
static uint8_t string_to_mode(const char *str) {
    uint8_t i;
    for (i = 1; i < NUM_ENCRYPTION_MODES; ++i) {
        if (strcmp(str, mode_strings[i]) == 0)
            return i;
    }
    return 0;
}

/*
 * Converts the encryption mode to a human-readable string. Returns "INVALID" if
 * the mode is not a valid encryption mode.
 */
static const char *mode_to_string(uint8_t mode) {
    if (mode >= NUM_ENCRYPTION_MODES)
        mode = 0;
    return mode_strings[mode];
}

/*
 * Converts an amount of padding (as a string) into the appropriate padding
 * flag. Returns -1 if the flag is invalid.
 */
static int string_to_padding_flag(const char *str) {
    int i, padding = atoi(str);
    for (i = 0; i < NUM_PADDING_VALUES; ++i) {
        if (padding == padding_values[i])
            return i;
    }
    return -1;
}
