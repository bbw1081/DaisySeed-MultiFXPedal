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