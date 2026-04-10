#include "PitchshifterEffect.h"

void PitchshifterEffect::Init(float sample_rate, const float* params){
    shift_.Init(sample_rate);
    shift_.SetDelSize(params[0]);
    shift_.SetFun(params[1]);
    shift_.SetTransposition(params[2]);
    mix_ = params[3];
}

float PitchshifterEffect::Process(float in) { 
    return (in * (1.0f - mix_)) + (shift_.Process(in) * mix_); 
}