#include "DecimatorEffect.h"

void DecimatorEffect::Init(float sample_rate, const float* params) {
    decimator_.Init();
    decimator_.SetBitsToCrush(params[0]);
    decimator_.SetDownsampleFactor(params[1]);
}

float DecimatorEffect::Process(float in) { return decimator_.Process(in); }