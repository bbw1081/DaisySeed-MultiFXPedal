#include "PhaserEffect.h"

void PhaserEffect::Init(float sample_rate, const float* params){
    phaser_.Init(sample_rate);
    phaser_.SetFeedback(params[0]);
    phaser_.SetFreq(params[1]);
    phaser_.SetLfoFreq(params[2]);
    phaser_.SetPoles(params[3]);
}

float PhaserEffect::Process(float in){ return phaser_.Process(in); }