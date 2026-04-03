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

    //update list to include any new effects
    AutowahEffect wah_;
    ChorusEffect chorus_;
    DecimatorEffect decimator_;
    EQEffect eq_;
    FlangerEffect flanger_;
    OverdriveEffect od_;
    PhaserEffect phaser_;
    PitchshifterEffect pitch_;
    TremoloEffect trem_;
    WavefolderEffect folder_;
    HardClipEffect hard_clip_;
    SoftClipEffect soft_clip_;
    JFETEffect jfet_;
    BJTEffect bjt_;
    OpAmpEffect opamp_;
    CMOSEffect cmos_;
};