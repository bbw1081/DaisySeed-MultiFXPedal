#pragma once
#include "BaseEffect.h"
#include "daisysp.h"

/**
 * Overdrive Effect Class
 * 
 * @author Richard Wilkinson
 */
class OverdriveEffect : public BaseEffect {
public:

    void Init(float sample_rate, const float* params) override;

    float Process(float in) override;

private:
    daisysp::Overdrive drive_;
};