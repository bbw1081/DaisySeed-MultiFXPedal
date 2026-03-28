#include "../dependencies/ArduinoJson-v7.4.3.h"
#include "EffectChain.h"

using namespace ArduinoJson;

static const int MAX_STRING = 32;
static const int MAX_PARAMS = 6;

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


private:
    char name_[MAX_STRING];

    char effects_[MAX_EFFECTS][MAX_STRING];
    int effect_count_;

    float params_[MAX_EFFECTS][MAX_PARAMS];
    int param_count_;

    EffectChain effect_chain_;

    OverdriveEffect od_;
};