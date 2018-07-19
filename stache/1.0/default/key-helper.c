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

#include "linux/fs.h"

#include "key-helper.h"

// Takes an input key descriptor as a byte array and outputs a hex string.
static void key_descriptor_to_hex(const uint8_t bytes[FS_KEY_DESCRIPTOR_SIZE],
                                  char hex[FS_KEY_DESCRIPTOR_HEX_SIZE]) {
    int i;
    for (i = 0; i < FS_KEY_DESCRIPTOR_SIZE; ++i)
        sprintf(hex + 2 * i, "%02x", bytes[i]);
}

// Takes an input key descriptor as a hex string and outputs a bytes array.
// Returns non-zero if the provided hex string is not formatted correctly.
static int key_descriptor_to_bytes(const char *hex,
                                   uint8_t bytes[FS_KEY_DESCRIPTOR_SIZE]) {
    if (strlen(hex) != FS_KEY_DESCRIPTOR_HEX_SIZE - 1)
        return -1;

    int i, bytes_converted, chars_read;
    for (i = 0; i < FS_KEY_DESCRIPTOR_SIZE; ++i) {
        // We must read two hex characters of input into one byte of buffer.
        bytes_converted = sscanf(hex + 2 * i, "%2hhx%n", bytes + i, &chars_read);
        if (bytes_converted != 1 || chars_read != 2)
            return -1;
    }
    return 0;
}

// Reads key data from stdin into the provided data buffer. Return 0 on success.
static int read_key(uint8_t key[FS_MAX_KEY_SIZE]) {
    size_t rc = fread(key, 1, FS_MAX_KEY_SIZE, stdin);
    int end = fgetc(stdin);
    // We should read exactly FS_MAX_KEY_SIZE bytes, then hit EOF
    if (rc == FS_MAX_KEY_SIZE && end == EOF && feof(stdin))
        return 0;

    fprintf(stderr, "error: input key must be %d bytes\n", FS_MAX_KEY_SIZE);
    return -1;
}

// The descriptor is just the first 8 bytes of a double application of SHA512
// formatted as hex (so 16 characters).
static void compute_descriptor(const uint8_t key[FS_MAX_KEY_SIZE],
                               char descriptor[FS_KEY_DESCRIPTOR_HEX_SIZE]) {
    uint8_t digest1[SHA512_DIGEST_LENGTH];
    SHA512(key, FS_MAX_KEY_SIZE, digest1);

    uint8_t digest2[SHA512_DIGEST_LENGTH];
    SHA512(digest1, SHA512_DIGEST_LENGTH, digest2);

    key_descriptor_to_hex(digest2, descriptor);
    secure_wipe(digest1, SHA512_DIGEST_LENGTH);
    secure_wipe(digest2, SHA512_DIGEST_LENGTH);
}

// Inserts the key into the current session keyring with type logon and the
// service specified by service_prefix.
static int insert_logon_key(const uint8_t key_data[FS_MAX_KEY_SIZE],
                            const char descriptor[FS_KEY_DESCRIPTOR_HEX_SIZE],
                            const char *service_prefix) {
    // We cannot add directly to KEY_SPEC_SESSION_KEYRING, as that will make a new
    // session keyring if one does not exist, rather than adding it to the user
    // session keyring.
    int keyring_id = keyctl_get_keyring_ID(KEY_SPEC_SESSION_KEYRING, 0);
    if (keyring_id < 0)
        return -1;

    char description[MAX_KEY_DESC_PREFIX_SIZE + FS_KEY_DESCRIPTOR_HEX_SIZE];
    sprintf(description, "%s%s", service_prefix, descriptor);

    struct fscrypt_key key = {.mode = 0, .size = FS_MAX_KEY_SIZE};
    memcpy(key.raw, key_data, FS_MAX_KEY_SIZE);

    int ret = add_key("logon", description, &key, sizeof(key), keyring_id) < 0 ? -1 : 0;

    secure_wipe(key.raw, FS_MAX_KEY_SIZE);
    return ret;
}
