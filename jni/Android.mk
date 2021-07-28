LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := multiplayer
LOCAL_LDLIBS := -ldl -landroid -llog -lOpenSLES

FILE_LIST += $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/VC/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/gui/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS += -w 
LOCAL_CPPFLAGS += -w -s -fvisibility=default -Wall -fpack-struct=1 -O2 -fstrict-aliasing -fno-omit-frame-pointer -mfloat-abi=soft -fstack-protector -fno-short-enums -frtti -g2 -gdwarf-2 -mthumb -ferror-limit=1 -fexceptions
LOCAL_CPPFLAGS += -DNDEBUG=1 -D__ANDROID_API__=14
LOCAL_LDFLAGS += -Wl,-z,relro -Wl,-z,noexecstack -Wl,--no-undefined -Wl,-z,now -Wl,--strip-all 

# LOCAL_CFLAGS := -Wno-error=format-security -fpermissive -fvisibility=hidden -fvisibility-inlines-hidden
# LOCAL_CFLAGS += -fno-rtti -fno-exceptions -g0 -fomit-frame-pointer -ffunction-sections -fdata-sections
# LOCAL_CPPFLAGS += -fvisibility=hidden -ffunction-sections -fdata-sections

# $(info "compiling shit..")
include $(BUILD_SHARED_LIBRARY)