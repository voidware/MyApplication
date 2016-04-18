
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += 

LOCAL_MODULE    := backend
#LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES := backend.cpp
LOCAL_LDLIBS    := -llog -ljnigraphics -lGLESv2
LOCAL_CPPFLAGS  +=  -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CFLAGS    +=  -fvisibility=hidden

include $(BUILD_SHARED_LIBRARY)
