/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "LogKlogCompat.h"

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/uio.h>
#include <syslog.h>

// Parsing is hard

// called if we see a '<', s is the next character, returns pointer after '>'
static char* is_prio(char* s, ssize_t len) {
    if ((len <= 0) || !isdigit(*s++)) return nullptr;
    --len;
    static const size_t max_prio_len = (len < 4) ? len : 4;
    size_t priolen = 0;
    char c;
    while (((c = *s++)) && (++priolen <= max_prio_len)) {
        if (!isdigit(c)) return ((c == '>') && (*s == '[')) ? s : nullptr;
    }
    return nullptr;
}

// called if we see a '[', s is the next character, returns pointer after ']'
static char* is_timestamp(char* s, ssize_t len) {
    while ((len > 0) && (*s == ' ')) {
        ++s;
        --len;
    }
    if ((len <= 0) || !isdigit(*s++)) return nullptr;
    --len;
    bool first_period = true;
    char c;
    while ((len > 0) && ((c = *s++))) {
        --len;
        if ((c == '.') && first_period) {
            first_period = false;
        } else if (!isdigit(c)) {
            return ((c == ']') && !first_period && (*s == ' ')) ? s : nullptr;
        }
    }
    return nullptr;
}

// Like strtok_r with "\r\n" except that we look for log signatures (regex)
//  \(\(<[0-9]\{1,4\}>\)\([[] *[0-9]+[.][0-9]+[]] \)\{0,1\}\|[[]
//  *[0-9]+[.][0-9]+[]] \)
// and split if we see a second one without a newline.
// We allow nuls in content, monitoring the overall length and sub-length of
// the discovered tokens.

#define SIGNATURE_MASK 0xF0
// <digit> following ('0' to '9' masked with ~SIGNATURE_MASK) added to signature
#define LESS_THAN_SIG SIGNATURE_MASK
#define OPEN_BRACKET_SIG ((SIGNATURE_MASK << 1) & SIGNATURE_MASK)
// space is one more than <digit> of 9
#define OPEN_BRACKET_SPACE ((char)(OPEN_BRACKET_SIG | 10))

char* android::log_strntok_r(char* s, ssize_t& len, char*& last,
                             ssize_t& sublen) {
    sublen = 0;
    if (len <= 0) return nullptr;
    if (!s) {
        if (!(s = last)) return nullptr;
        // fixup for log signature split <,
        // LESS_THAN_SIG + <digit>
        if ((*s & SIGNATURE_MASK) == LESS_THAN_SIG) {
            *s = (*s & ~SIGNATURE_MASK) + '0';
            *--s = '<';
            ++len;
        }
        // fixup for log signature split [,
        // OPEN_BRACKET_SPACE is space, OPEN_BRACKET_SIG + <digit>
        if ((*s & SIGNATURE_MASK) == OPEN_BRACKET_SIG) {
            *s = (*s == OPEN_BRACKET_SPACE) ? ' ' : (*s & ~SIGNATURE_MASK) + '0';
            *--s = '[';
            ++len;
        }
    }

    while ((len > 0) && ((*s == '\r') || (*s == '\n'))) {
        ++s;
        --len;
    }

    if (len <= 0) return last = nullptr;
    char *peek, *tok = s;

    for (;;) {
        if (len <= 0) {
            last = nullptr;
            return tok;
        }
        char c = *s++;
        --len;
        ssize_t adjust;
        switch (c) {
            case '\r':
            case '\n':
                s[-1] = '\0';
                last = s;
                return tok;

            case '<':
                peek = is_prio(s, len);
                if (!peek) break;
                if (s != (tok + 1)) {  // not first?
                    s[-1] = '\0';
                    *s &= ~SIGNATURE_MASK;
                    *s |= LESS_THAN_SIG;  // signature for '<'
                    last = s;
                    return tok;
                }
                adjust = peek - s;
                if (adjust > len) {
                    adjust = len;
                }
                sublen += adjust;
                len -= adjust;
                s = peek;
                if ((*s == '[') && ((peek = is_timestamp(s + 1, len - 1)))) {
                    adjust = peek - s;
                    if (adjust > len) {
                        adjust = len;
                    }
                    sublen += adjust;
                    len -= adjust;
                    s = peek;
                }
                break;

            case '[':
                peek = is_timestamp(s, len);
                if (!peek) break;
                if (s != (tok + 1)) {  // not first?
                    s[-1] = '\0';
                    if (*s == ' ') {
                        *s = OPEN_BRACKET_SPACE;
                    } else {
                        *s &= ~SIGNATURE_MASK;
                        *s |= OPEN_BRACKET_SIG;  // signature for '['
                    }
                    last = s;
                    return tok;
                }
                adjust = peek - s;
                if (adjust > len) {
                    adjust = len;
                }
                sublen += adjust;
                len -= adjust;
                s = peek;
                break;
        }
        ++sublen;
    }
    // NOTREACHED
}
