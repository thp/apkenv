LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	main_surfaceflinger.cpp 

LOCAL_SHARED_LIBRARIES := \
	libsurfaceflinger \
	libbinder \
	libutils

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../services/surfaceflinger

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libsurfaceflinger_compat

include $(BUILD_SHARED_LIBRARY)
