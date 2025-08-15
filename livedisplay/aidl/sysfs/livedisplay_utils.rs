/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

use binder::{Result as BinderResult, StatusCode};
use std::fs;

pub fn is_path_accessible(path: &str) -> bool {
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .open(path)
        .is_ok()
}

pub fn get_array(path: &str) -> BinderResult<Vec<i32>> {
    fs::read_to_string(path)
        .map_err(|err| {
            log::error!("Failed to read from {}: {}", path, err);
            StatusCode::UNKNOWN_ERROR.into()
        })
        .and_then(|s| {
            s.trim()
                .trim_end_matches('\0')
                .split_whitespace()
                .map(|s| {
                    s.parse::<i32>().map_err(|err| {
                        log::error!("Failed to parse number: {}", err);
                        StatusCode::UNKNOWN_ERROR.into()
                    })
                })
                .collect()
        })
}

pub fn get_enabled(path: &str) -> BinderResult<bool> {
    fs::read_to_string(path)
        .map_err(|err| {
            log::error!("Failed to read from {}: {}", path, err);
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

pub fn set_array(path: &str, values: &[i32]) -> BinderResult<()> {
    let s = values
        .iter()
        .map(|&i| i.to_string())
        .collect::<Vec<String>>()
        .join(" ");

    fs::write(path, s).map_err(|err| {
        log::error!("Failed to write to file {}: {}", path, err);
        StatusCode::UNKNOWN_ERROR.into()
    })
}

pub fn set_enabled_bool(path: &str, enabled: bool) -> BinderResult<()> {
    fs::write(path, if enabled { "1" } else { "0" }).map_err(|err| {
        log::error!("Failed to write to {}: {}", path, err);
        StatusCode::UNKNOWN_ERROR.into()
    })
}

pub fn set_enabled_mode(path: &str, enabled_mode: i32) -> BinderResult<()> {
    fs::write(path, enabled_mode.to_string()).map_err(|err| {
        log::error!("Failed to write to {}: {}", path, err);
        StatusCode::UNKNOWN_ERROR.into()
    })
}
