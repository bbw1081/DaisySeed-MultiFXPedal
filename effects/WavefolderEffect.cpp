#include "WavefolderEffect.h"

void WavefolderEffect::Init(float sample_rate, const float* params) {
    folder_.Init();
    folder_.SetGain(params[0]);
    folder_.SetOffset(params[1]);
}

float WavefolderEffect::Process(float in) { return folder_.Process(in); }