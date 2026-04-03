#pragma once

#include "BaseEffect.h"
#include <cmath>

/**
 * JFET-style Distortion Effect (HSP_Protoseed inspired)
 * Square-law soft knee with bias-driven asymmetry
 * 
 * Parameters: [drive, grit, bias, tone]
 * - drive: 0-1 (input gain)
 * - grit: 0-1 (soft knee steepness)
 * - bias: 0-1 (asymmetry control)
 * - tone: 0-1 (tone control, LPF cutoff)
 */
class JFETEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    float sample_rate_;
    float drive_;
    float grit_;
    float bias_;
    float tone_blend_old_;
    float tone_blend_new_;
    float tone_memory_;
};
