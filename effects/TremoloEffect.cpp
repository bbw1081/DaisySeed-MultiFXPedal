#include "TremoloEffect.h"

void TremoloEffect::Init(float sample_rate, const float* params){
    trem_.Init(sample_rate);
    trem_.SetDepth(params[0]);
    trem_.SetFreq(params[1]);
}

float TremoloEffect::Process(float in) { return trem_.Process(in); }