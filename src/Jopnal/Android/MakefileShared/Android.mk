LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := jopnal
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL -llog -landroid
LOCAL_SHARED_LIBRARIES := openal

prebuilt_path := $(call local-prebuilt-path, $(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_SHARED_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnal-main
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal-main.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

prebuilt_path := $(call local-prebuilt-path, $(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_STATIC_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnald
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnald.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL -llog -landroid
LOCAL_SHARED_LIBRARIES := openal

prebuilt_path := $(call local-prebuilt-path, $(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_SHARED_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnal-maind
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal-maind.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

prebuilt_path := $(call local-prebuilt-path, $(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_STATIC_LIBRARY)
endif


$(call import-module, jopnal/extlibs)