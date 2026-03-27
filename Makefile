# Project Name
TARGET = pedal

# Sources
CPP_SOURCES = pedal.cpp \
effects/OverdriveEffect.cpp \
Preset.cpp \
PresetManager.cpp

# Library Locations
LIBDAISY_DIR = ../../../Desktop/DaisyExamples/libDaisy/
DAISYSP_DIR = ../../../Desktop/DaisyExamples/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# allows for serial printing of floats, uncomment when debugging adc
# LDFLAGS += -u _printf_float
