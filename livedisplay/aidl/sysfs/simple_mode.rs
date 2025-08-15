/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

use binder::{Interface, Result as BinderResult};

use vendor_lineage_livedisplay::aidl::vendor::lineage::livedisplay::{
    IAutoContrast::IAutoContrast, IColorEnhancement::IColorEnhancement,
    IReadingEnhancement::IReadingEnhancement,
};

use crate::livedisplay_utils;

pub trait SimpleModeTrait {
    const NODE: &'static str;

    fn is_supported(&self) -> bool {
        livedisplay_utils::is_path_accessible(Self::NODE)
    }
}

macro_rules! define_livedisplay_feature {
    (
        $service_name:ident,
        $interface_name:ty,
        $node_path:expr
    ) => {
        pub struct $service_name;

        impl SimpleModeTrait for $service_name {
            const NODE: &'static str = $node_path;
        }

        impl Interface for $service_name {}

        impl $interface_name for $service_name {
            fn getEnabled(&self) -> BinderResult<bool> {
                livedisplay_utils::get_enabled(Self::NODE)
            }

            fn setEnabled(&self, enabled: bool) -> BinderResult<()> {
                livedisplay_utils::set_enabled_bool(Self::NODE, enabled)
            }
        }
    };
}

define_livedisplay_feature!(AutoContrast, IAutoContrast, "/sys/class/graphics/fb0/aco");
define_livedisplay_feature!(
    ColorEnhancement,
    IColorEnhancement,
    "/sys/class/graphics/fb0/color_enhance"
);
define_livedisplay_feature!(
    ReadingEnhancement,
    IReadingEnhancement,
    "/sys/class/graphics/fb0/reading_mode"
);
