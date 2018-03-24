#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32_led_panel_i2c
CFLAGS=-Wno-error=maybe-uninitialized

include $(IDF_PATH)/make/project.mk
