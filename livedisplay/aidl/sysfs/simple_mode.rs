/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

use binder::{Interface, Result as BinderResult};

use vendor_lineage_livedisplay::aidl::vendor::lineage::livedisplay::{
    IAutoContrast::IAutoContrast,
//    IColorEnhancement::IColorEnhancement,
//    IReadingEnhancement::IReadingEnhancement,
};

use crate::livedisplay_utils;

pub struct AutoContrast;

impl AutoContrast {
    const NODE: &str = "/sys/class/graphics/fb0/aco";

    pub fn is_supported(&self) -> bool {
        livedisplay_utils::is_path_accessible(Self::NODE)
    }
}

impl Interface for AutoContrast {}

impl IAutoContrast for AutoContrast {
    fn getEnabled(&self) -> BinderResult<bool> {
        livedisplay_utils::get_enabled(Self::NODE)
    }

    fn setEnabled(&self, enabled: bool) -> BinderResult<()> {
        livedisplay_utils::set_enabled_bool(Self::NODE, enabled)
    }
}
