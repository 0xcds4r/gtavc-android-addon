LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := multiplayer
LOCAL_LDLIBS := -ldl -landroid -llog -lOpenSLES

FILE_LIST += $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/VC/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_CFLAGS := -w
LOCAL_CPPFLAGS := -w -s -fvisibility=default -Wall -fpack-struct=1 -O2 -std=c++17 -fstrict-aliasing -fno-omit-frame-pointer -mfloat-abi=soft -fstack-protector -fno-short-enums -frtti -g2 -gdwarf-2 -mthumb -ferror-limit=1 -fexceptions

include $(BUILD_SHARED_LIBRARY)