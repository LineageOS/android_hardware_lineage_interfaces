#!/bin/bash

HIDL_PACKAGES=(
  "vendor.lineage.camera.motor@1.0"
  "vendor.lineage.fastcharge@1.0"
  "vendor.lineage.livedisplay@2.0"
  "vendor.lineage.livedisplay@2.1"
  "vendor.lineage.powershare@1.0"
  "vendor.lineage.touch@1.0"
  "vendor.lineage.trust@1.0"
)

for hidl_package in "${HIDL_PACKAGES[@]}"; do
  hidl-gen -L hash -r vendor.lineage:hardware/lineage/interfaces -r android.hardware:hardware/interfaces -r android.hidl:system/libhidl/transport "$hidl_package"
done
