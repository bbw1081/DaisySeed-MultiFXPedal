#include "FlangerEffect.h"

void FlangerEffect::Init(float sample_rate, const float* params){
    flanger_.Init(sample_rate);
    flanger_.SetDelayMs(params[0]);
    flanger_.SetFeedback(params[1]);
    flanger_.SetLfoFreq(params[2]);
}

float FlangerEffect::Process(float in) { return flanger_.Process(in); }