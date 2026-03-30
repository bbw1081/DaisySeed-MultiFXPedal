#include "OverdriveEffect.h"

void OverdriveEffect::Init(float sample_rate, const float* params) {
    drive_.Init();
    drive_.SetDrive(params[0]);
}

float OverdriveEffect::Process(float in) { return drive_.Process(in); }
