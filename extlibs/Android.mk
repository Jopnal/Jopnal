LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libopenal.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := bulletcollision
ifeq ($(APP_OPTIM),debug)
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libBulletCollision.a
else
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libBulletCollisiond.a
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := bulletdynamics
ifeq ($(APP_OPTIM),debug)
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libBulletDynamics.a
else
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libBulletDynamicsd.a
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := bulletsoftbody
ifeq ($(APP_OPTIM),debug)
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libBulletSoftBody.a
else
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libBulletSoftBodyd.a
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := linearmath
ifeq ($(APP_OPTIM),debug)
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libLinearMath.a
else
    LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libLinearMathd.a
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := physfs
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libphysfs.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)