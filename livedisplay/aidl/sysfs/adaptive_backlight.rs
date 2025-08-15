/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

use binder::{Interface, Result as BinderResult};

use vendor_lineage_livedisplay::aidl::vendor::lineage::livedisplay::IAdaptiveBacklight::IAdaptiveBacklight;

use crate::livedisplay_utils;

const FILE_ACL: &str = "/sys/class/graphics/fb0/acl";
const FILE_CABC: &str = "/sys/class/graphics/fb0/cabc";

pub struct AdaptiveBacklight {
    file: Option<String>,
}

impl AdaptiveBacklight {
    pub fn new() -> Self {
        if livedisplay_utils::is_path_accessible(FILE_ACL) {
            Self {
                file: Some(FILE_ACL.to_string()),
            }
        } else if livedisplay_utils::is_path_accessible(FILE_CABC) {
            Self {
                file: Some(FILE_CABC.to_string()),
            }
        } else {
            Self { file: None }
        }
    }

    pub fn is_supported(&self) -> bool {
        self.file.is_some()
    }
}

impl Interface for AdaptiveBacklight {}

impl IAdaptiveBacklight for AdaptiveBacklight {
    fn getEnabled(&self) -> BinderResult<bool> {
        livedisplay_utils::get_enabled(&self.file.clone().unwrap())
    }

    fn setEnabled(&self, enabled: bool) -> BinderResult<()> {
        livedisplay_utils::set_enabled_bool(&self.file.clone().unwrap(), enabled)
    }
}
