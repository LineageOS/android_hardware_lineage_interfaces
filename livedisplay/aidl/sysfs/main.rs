/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

//! This implements the Lineage LiveDisplay sysfs HAL.
mod adaptive_backlight;
mod livedisplay_utils;
mod simple_mode;
mod sunlight_enhancement;

use log::LevelFilter;

use vendor_lineage_livedisplay::aidl::vendor::lineage::livedisplay::{
    IAdaptiveBacklight::BnAdaptiveBacklight, IAdaptiveBacklight::IAdaptiveBacklight,
    IAutoContrast::BnAutoContrast, IAutoContrast::IAutoContrast,
    ISunlightEnhancement::BnSunlightEnhancement, ISunlightEnhancement::ISunlightEnhancement,
};

use crate::adaptive_backlight::AdaptiveBacklight;
use crate::simple_mode::AutoContrast;
use crate::sunlight_enhancement::SunlightEnhancement;

const LOG_TAG: &str = "vendor.lineage.livedisplay-service-sysfs";

fn register_as_services() {
    let my_service = AdaptiveBacklight::new();
    if my_service.is_supported() {
        let my_service_binder =
            BnAdaptiveBacklight::new_binder(my_service, binder::BinderFeatures::default());
        let service_name = format!("{}/default", AdaptiveBacklight::get_descriptor());
        binder::add_service(&service_name, my_service_binder.as_binder())
            .expect("Failed to register AdaptiveBacklight service");
    }

    let my_service = AutoContrast;
    if my_service.is_supported() {
        let my_service_binder =
            BnAutoContrast::new_binder(my_service, binder::BinderFeatures::default());
        let service_name = format!("{}/default", AutoContrast::get_descriptor());
        binder::add_service(&service_name, my_service_binder.as_binder())
            .expect("Failed to register AutoContrast service");
    }

    let my_service = SunlightEnhancement::new();
    if my_service.is_supported() {
        let my_service_binder =
            BnSunlightEnhancement::new_binder(my_service, binder::BinderFeatures::default());
        let service_name = format!("{}/default", SunlightEnhancement::get_descriptor());
        binder::add_service(&service_name, my_service_binder.as_binder())
            .expect("Failed to register SunlightEnhancement service");
    }
}

fn main() {
    let logger_success = logger::init(
        logger::Config::default()
            .with_tag_on_device(LOG_TAG)
            .with_max_level(LevelFilter::Trace),
    );
    if !logger_success {
        panic!("{LOG_TAG}: Failed to start logger.");
    }

    binder::ProcessState::start_thread_pool();

    register_as_services();

    // Does not return.
    binder::ProcessState::join_thread_pool()
}
