/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

use binder::{Interface, Result as BinderResult};

use vendor_lineage_livedisplay::aidl::vendor::lineage::livedisplay::IDisplayColorCalibration::IDisplayColorCalibration;

use crate::livedisplay_utils;

const FILE_RGB: &str = "/sys/class/graphics/fb0/rgb";

pub struct DisplayColorCalibration;

impl DisplayColorCalibration {
    pub fn is_supported(&self) -> bool {
        livedisplay_utils::is_path_accessible(FILE_RGB)
    }
}

impl Interface for DisplayColorCalibration {}

impl IDisplayColorCalibration for DisplayColorCalibration {
    fn getMaxValue(&self) -> BinderResult<i32> {
        Ok(32768)
    }

    fn getMinValue(&self) -> BinderResult<i32> {
        Ok(255)
    }

    fn getCalibration(&self) -> BinderResult<Vec<i32>> {
        livedisplay_utils::get_array(FILE_RGB)
    }

    fn setCalibration(&self, rgb: &[i32]) -> BinderResult<bool> {
        livedisplay_utils::set_array(FILE_RGB, rgb)
    }
}
