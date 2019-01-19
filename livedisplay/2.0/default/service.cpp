/*
 * Copyright (C) 2017-2019 The LineageOS Project
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

#if defined(COLOR_BACKEND_SDM)
#define LOG_TAG "vendor.lineage.livedisplay@2.0-service-sdm"
#elif defined(COLOR_BACKEND_LEGACYMM)
#define LOG_TAG "vendor.lineage.livedisplay@2.0-service-legacymm"
#else
#error "Color backend undefined!"
#endif

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "AdaptiveBacklight.h"
#include "ColorBalance.h"
#include "DisplayModes.h"
#include "PictureAdjustment.h"
#include "SunlightEnhancement.h"

using android::OK;
using android::sp;
using android::status_t;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using ::vendor::lineage::livedisplay::V2_0::implementation::Color;
using ::vendor::lineage::livedisplay::V2_0::implementation::AdaptiveBacklight;
using ::vendor::lineage::livedisplay::V2_0::implementation::ColorBalance;
using ::vendor::lineage::livedisplay::V2_0::implementation::DisplayModes;
using ::vendor::lineage::livedisplay::V2_0::implementation::PictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::implementation::SunlightEnhancement;

int main() {
    sp<AdaptiveBacklight> ab;
    sp<ColorBalance> cb;
    sp<DisplayModes> dm;
    sp<PictureAdjustment> pa;
    sp<SunlightEnhancement> se;
    status_t status;

    LOG(INFO) << "LiveDisplay HAL service is starting.";

#ifdef LIVES_IN_SYSTEM
    // The LiveDisplay HAL may communicate to other components via /dev/binder
    android::ProcessState::initWithDriver("/dev/binder");
#else
    // The LiveDisplay HAL may communicate to other vendor components via /dev/vndbinder
    android::ProcessState::initWithDriver("/dev/vndbinder");
#endif

    sp<Color> colorImpl = new Color();
    if (colorImpl == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    ab = new AdaptiveBacklight(colorImpl);
    if (ab == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL AdaptiveBacklight Iface, exiting.";
        goto shutdown;
    }
    if (ab->isSupported()) {
        status = ab->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL AdaptiveBacklight Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    cb = new ColorBalance(colorImpl);
    if (cb == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL ColorBalance Iface, exiting.";
        goto shutdown;
    }
    if (cb->isSupported()) {
        status = cb->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL ColorBalance Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    dm = new DisplayModes(colorImpl);
    if (dm == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayModes Iface, exiting.";
        goto shutdown;
    }
    if (dm->isSupported()) {
        status = dm->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL DisplayModes Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    pa = new PictureAdjustment(colorImpl);
    if (pa == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL PictureAdjustment Iface, exiting.";
        goto shutdown;
    }
    if (pa->isSupported()) {
        status = pa->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL PictureAdjustment Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    se = new SunlightEnhancement(colorImpl);
    if (se == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL SunlightEnhancement Iface, exiting.";
        goto shutdown;
    }
    if (se->isSupported()) {
        status = se->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for LiveDisplay HAL SunlightEnhancement Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to exit
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}
