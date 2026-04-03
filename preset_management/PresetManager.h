#pragma once

#include "Preset.h"
#include "../dependencies/ArduinoJson-v7.4.3.h"

#include "../presets/Presets.h"

static const int MAX_PRESETS = 20;

/**
 * Class to manage currently selected preset
 * 
 * @author Richard Wilkinson
 */
class PresetManager {
public:

    /**
     * Initializes the preset manager
     * 
     * @param sample_rate the audio sample rate
     */
    void Init(int sample_rate);

    /**
     * Changes the current preset number
     * 
     * @param val either -1 or 1, depending on whether you're moving forward or backing up 1 preset
     */
    void ChangePreset(int val);

    /**
     * Processes audio according to active preset
     * 
     * @param in input audio sample
     * @return the processed audio
     */
    float Process(float in);

    /**
     * Returns the name of the current active preset
     * 
     * @return the name of the current preset
     */
    const char* GetName();


private:
    int num_presets_;
    int sample_rate_;
    int current_preset_num_;
    Preset current_preset_;

    const char* preset_data_[MAX_PRESETS];

    /**
     * Sets the active preset to the value input
     * 
     * @param val the preset number to set
     */
    void SetActivePreset(int val);
};