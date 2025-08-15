/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

use binder::{Interface, Result as BinderResult, StatusCode};
use std::fs;

use vendor_lineage_livedisplay::aidl::vendor::lineage::livedisplay::ISunlightEnhancement::ISunlightEnhancement;

use crate::livedisplay_utils;

const FILE_HBM: &str = "/sys/class/graphics/fb0/hbm";
const FILE_SRE: &str = "/sys/class/graphics/fb0/sre";

pub struct SunlightEnhancement {
    file: Option<String>,
    enabled_mode: i32,
}

impl SunlightEnhancement {
    pub fn new() -> Self {
        if livedisplay_utils::is_path_accessible(FILE_HBM) {
            Self {
                file: Some(FILE_HBM.to_string()),
                enabled_mode: 1,
            }
        } else if livedisplay_utils::is_path_accessible(FILE_SRE) {
            Self {
                file: Some(FILE_SRE.to_string()),
                enabled_mode: 2,
            }
        } else {
            Self {
                file: None,
                enabled_mode: 0,
            }
        }
    }

    pub fn is_supported(&self) -> bool {
        self.file.is_some()
    }
}

impl Interface for SunlightEnhancement {}

impl ISunlightEnhancement for SunlightEnhancement {
    fn getEnabled(&self) -> BinderResult<bool> {
        fs::read_to_string(self.file.clone().unwrap())
            .map_err(|err| {
                log::error!(
                    "Failed to read from {}: {}",
                    self.file.clone().unwrap(),
                    err
                );
                StatusCode::UNKNOWN_ERROR.into()
            })
            .and_then(|s| {
                s.trim()
                    .trim_end_matches('\0')
                    .parse::<i32>()
                    .map(|value| value > 0)
                    .map_err(|err| {
                        log::error!("Failed to parse number: {}", err);
                        StatusCode::UNKNOWN_ERROR.into()
                    })
            })
    }

    fn setEnabled(&self, enabled: bool) -> BinderResult<()> {
        fs::write(
            self.file.clone().unwrap(),
            if enabled {
                self.enabled_mode.to_string()
            } else {
                "0".to_string()
            },
        )
        .map_err(|err| {
            log::error!("Failed to write to {}: {}", self.file.clone().unwrap(), err);
            StatusCode::UNKNOWN_ERROR.into()
        })
    }
}
