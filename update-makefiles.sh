#!/bin/bash

source system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update \
  "vendor.lineage:hardware/lineage/interfaces" \
  "android.hidl:system/libhidl/transport"
