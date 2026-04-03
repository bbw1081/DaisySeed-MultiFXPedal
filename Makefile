# Project Name
TARGET = pedal

# Optimization: Use -Os (optimize for size) instead of default -O2 (optimize for speed)
# This helps fit all the distortion effects into the limited FLASH memory
OPT = -Os

# Sources
CPP_SOURCES = pedal.cpp \
effects/AutowahEffect.cpp \
effects/BJTEffect.cpp \
effects/ChorusEffect.cpp \
effects/CMOSEffect.cpp \
effects/DecimatorEffect.cpp \
effects/EQEffect.cpp \
effects/FlangerEffect.cpp \
effects/HardClipEffect.cpp \
effects/JFETEffect.cpp \
effects/OpAmpEffect.cpp \
effects/OverdriveEffect.cpp \
effects/PhaserEffect.cpp \
effects/PitchshifterEffect.cpp \
effects/SoftClipEffect.cpp \
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

