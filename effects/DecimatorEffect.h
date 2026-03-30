#pragma once

#include "BaseEffect.h"

class DecimatorEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    daisysp::Decimator decimator_;
};