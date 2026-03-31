# Project Name
TARGET = pedal

# Sources
CPP_SOURCES = pedal.cpp \
effects/AutowahEffect.cpp \
effects/ChorusEffect.cpp \
effects/DecimatorEffect.cpp \
effects/FlangerEffect.cpp \
effects/OverdriveEffect.cpp \
effects/PhaserEffect.cpp \
effects/PitchshifterEffect.cpp \
effects/TremoloEffect.cpp \
effects/WavefolderEffect.cpp \
preset_management/Preset.cpp \
preset_management/PresetManager.cpp

# Library Locations
LIBDAISY_DIR = dependencies/libDaisy/
DAISYSP_DIR = dependencies/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

