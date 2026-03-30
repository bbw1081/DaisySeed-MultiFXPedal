#pragma once

#include "BaseEffect.h"

class FlangerEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    daisysp::Flanger flanger_;
};