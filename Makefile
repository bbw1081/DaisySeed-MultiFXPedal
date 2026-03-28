# Project Name
TARGET = pedal

# Sources
CPP_SOURCES = pedal.cpp \
effects/OverdriveEffect.cpp \
preset_management/Preset.cpp \
preset_management/PresetManager.cpp

# Library Locations
LIBDAISY_DIR = dependencies/libDaisy/
DAISYSP_DIR = dependencies/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# allows for serial printing of floats, uncomment when debugging adc
# LDFLAGS += -u _printf_float
