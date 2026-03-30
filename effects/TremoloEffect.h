#pragma once

#include "BaseEffect.h"

class TremoloEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    daisysp::Tremolo trem_;
};