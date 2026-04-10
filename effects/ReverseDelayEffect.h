#pragma once

#include "BaseEffect.h"
#include "../dependencies/delayline_reverse.h"

class ReverseDelayEffect : public BaseEffect{
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    daisysp::DelayLineReverse<float, 48000>* delay_;

    float sample_rate_;
    float feedback_;
    float mix_;
};