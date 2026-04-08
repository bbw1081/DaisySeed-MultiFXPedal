#pragma once

#include "BaseEffect.h"
#include "../dependencies/DaisySP/Source/Utility/delayline.h"

class DelayEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;

    float Process(float in) override;

private:

    //max delay currently set to 1 second, increase value for longer delay times
    daisysp::DelayLine<float, 48000>* delay_;

    float sample_rate_;
    float feedback_;
    float mix_;
};