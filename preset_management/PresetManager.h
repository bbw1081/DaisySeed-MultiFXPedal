/**
    PresetManager.h, contains a class that manages presets and switching between them
    Copyright (C) 2026  Richard Wilkinson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include "Preset.h"
#include "../dependencies/ArduinoJson-v7.4.3.h"
#include "../effects/Effects.h"

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

    // Shared effect instances (instantiated once, reused across presets)
    AutowahEffect wah_;
    BJTEffect bjt_;
    ChorusEffect chorus_;
    CMOSEffect cmos_;
    DecimatorEffect decimator_;
    EQEffect eq_;
    FlangerEffect flanger_;
    HardClipEffect hard_clip_;
    JFETEffect jfet_;
    OpAmpEffect opamp_;
    OverdriveEffect od_;
    PhaserEffect phaser_;
    PitchshifterEffect pitch_;
    SoftClipEffect soft_clip_;
    TremoloEffect trem_;
    WavefolderEffect folder_;
    DelayEffect delay_;
    ReverbEffect reverb_;
    ReverseDelayEffect revdelay_;


    /**
     * Sets the active preset to the value input
     * 
     * @param val the preset number to set
     */
    void SetActivePreset(int val);

    friend class Preset;  // Allow Preset to access shared effect instances
};