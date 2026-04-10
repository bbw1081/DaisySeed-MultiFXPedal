/**
    Preset.h, contains a class to represents a single preset
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
#include "../dependencies/ArduinoJson-v7.4.3.h"
#include "EffectChain.h"

using namespace ArduinoJson;

static const int MAX_STRING = 32;
static const int MAX_PARAMS = 10;

/**
 * Class to represent a single preset
 * 
 * @author Richard Wilkinson
 */
class Preset {
public:

    /**
     * Initilize the preset
     * 
     * @param doc Json document containing the preset data
     * @param sample_rate audio sample rate
     */
    void Init(JsonDocument doc, int sample_rate);

    /**
     * Process audio
     * 
     * @param in the audio sample
     * @return processed audio
     */
    float Process(float in);

    /**
     * Returns the preset's name
     * 
     * @return the preset's name
     */
    const char* GetName();

private:
    char name_[MAX_STRING];

    char effects_[MAX_EFFECTS][MAX_STRING];
    int effect_count_;

    float params_[MAX_EFFECTS][MAX_PARAMS];
    int param_count_;

    EffectChain effect_chain_;

    // Pointers to shared effect instances (owned by PresetManager)
    AutowahEffect* wah_;
    BJTEffect* bjt_;
    ChorusEffect* chorus_;
    CMOSEffect* cmos_;
    DecimatorEffect* decimator_;
    EQEffect* eq_;
    FlangerEffect* flanger_;
    HardClipEffect* hard_clip_;
    JFETEffect* jfet_;
    OpAmpEffect* opamp_;
    OverdriveEffect* od_;
    PhaserEffect* phaser_;
    PitchshifterEffect* pitch_;
    SoftClipEffect* soft_clip_;
    TremoloEffect* trem_;
    WavefolderEffect* folder_;
    DelayEffect* delay_;
    ReverbEffect* reverb_;
    ReverseDelayEffect* revdelay_;

    friend class PresetManager;  // Allow PresetManager to set pointers
};