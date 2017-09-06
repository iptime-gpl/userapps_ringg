$(info include $(notdir $(lastword $(MAKEFILE_LIST))))

KERNEL_PATH:=../../linux/linux-3.10

ifeq ($(SDK_VER),)
ifeq ($(USE_RTL_SDK_3411B),y)
SDK_VER:=3.4.11b
else
ifeq ($(USE_RTL_SDK_3465),y)
SDK_VER:=3.4.6.5
else
SDK_VER:=3.4.6.4
endif
endif
endif
