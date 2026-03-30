#include "ChorusEffect.h"

void ChorusEffect::Init(float sample_rate, const float* params){
    chorus_.Init(sample_rate);
    chorus_.SetDelayMs(params[0]);
    chorus_.SetFeedback(params[1]);
    chorus_.SetLfoFreq(params[2]);
}

float ChorusEffect::Process(float in){ return chorus_.Process(in); }