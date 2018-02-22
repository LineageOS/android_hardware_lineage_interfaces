/*
 * Copyright (C) 2017-2018 The LineageOS Project
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

#define LOG_NDEBUG 0

#define LOG_TAG "LiveDisplay-HIDL"

#include <log/log.h>
#include <stdarg.h>

#include "Color.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::OK;
using ::android::NO_INIT;

Color::Color() : mConnected(false) {
    ALOGD("Loaded LiveDisplay native interface");
}

Color::~Color() {
    reset();
}

void Color::reset() {
    if (mConnected) {
        deinitialize();
    }
    mFeatures = 0;
    mConnected = false;
}

void Color::error(const char* msg, ...) {
    if (msg != NULL) {
        va_list args;
        ALOGE(msg, args);
    }

    reset();
}

bool Color::connect() {
    if (mConnected) {
        return true;
    }

    mFeatures = 0;

    if (initialize() != OK) {
        ALOGE("Failed to initialize backend!");
        return false;
    }

    for (uint32_t i = 1; i <= (uint32_t)Feature::MAX; i <<= 1) {
        Feature f = static_cast<Feature>(i);
        if (backendHasFeature(f)) {
            addFeature(f);
        }
    }
    mConnected = true;

    return mFeatures > 0;
}

bool Color::check(Feature f) {
    return hasFeature(f) && connect();
}

Return<Features> Color::native_getSupportedFeatures() {
    connect();
    return mFeatures;
}

} // namespace implementation
} // namespace V1_0
} // namespace livedisplay
} // namespace lineage
} // namespace vendor
