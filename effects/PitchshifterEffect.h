#pragma once

#include "BaseEffect.h"

class PitchshifterEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    daisysp::PitchShifter shift_;

    float mix_;
};