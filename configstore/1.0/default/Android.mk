LOCAL_PATH := $(call my-dir)

################################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := vendor.lineage.configstore@1.0-service
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_INIT_RC := vendor.lineage.configstore@1.0-service.rc
LOCAL_SRC_FILES:= service.cpp

include $(LOCAL_PATH)/omx.mk

LOCAL_SHARED_LIBRARIES := \
    vendor.lineage.configstore@1.0 \
    libhidlbase \
    libhidltransport \
    libbase \
    libhwminijail \
    liblog \
    libutils \

include $(BUILD_EXECUTABLE)
