LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := jopnal
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnals.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL -llog -landroid -lGLESv3
LOCAL_WHOLE_STATIC_LIBRARIES := bullet physfs
LOCAL_SHARED_LIBRARIES := openal

prebuilt_path := $(call local-prebuilt-path,$(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_STATIC_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnal-main
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal-main.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_WHOLE_STATIC_LIBRARIES := jopnal

prebuilt_path := $(call local-prebuilt-path,$(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_STATIC_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnal-activity
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal-activity.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

prebuilt_path := $(call local-prebuilt-path,$(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_SHARED_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnald
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnalsd.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL -llog -landroid -lGLESv3
LOCAL_WHOLE_STATIC_LIBRARIES := bullet physfs
LOCAL_SHARED_LIBRARIES := openal

prebuilt_path := $(call local-prebuilt-path,$(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_STATIC_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnal-maind
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal-maind.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_WHOLE_STATIC_LIBRARIES := jopnal

prebuilt_path := $(call local-prebuilt-path,$(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_STATIC_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := jopnal-activityd
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libjopnal-activityd.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

prebuilt_path := $(call local-prebuilt-path,$(LOCAL_SRC_FILES))
prebuilt := $(strip $(wildcard $(prebuilt_path)))

ifdef prebuilt
    include $(PREBUILT_SHARED_LIBRARY)
endif


$(call import-module, jopnal/extlibs)