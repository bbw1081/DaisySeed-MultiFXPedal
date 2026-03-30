#include "AutowahEffect.h"

void AutowahEffect::Init(float sample_rate, const float* params) {
    autowah_.Init(sample_rate);

    autowah_.SetDryWet(params[0]);
    autowah_.SetLevel(params[1]);
    autowah_.SetWah(params[2]);
}

float AutowahEffect::Process(float in) { return autowah_.Process(in); }
