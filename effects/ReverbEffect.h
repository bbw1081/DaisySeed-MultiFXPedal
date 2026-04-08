#pragma once

#include "BaseEffect.h"
#include "../dependencies/DaisySP/DaisySP-LGPL/Source/Effects/reverbsc.h"

class ReverbEffect : public BaseEffect {
public:

    void Init(float sample_rate, const float* params) override;

    float Process(float in) override;

private:
    daisysp::ReverbSc* reverb_;

    float outL_;
    float outR_;
};