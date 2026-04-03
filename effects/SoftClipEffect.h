#pragma once

#include "BaseEffect.h"
#include <cmath>

/**
 * Soft Clipping Distortion Effect (HSP_Protoseed inspired)
 * Smooth saturation using hyperbolic tangent
 * 
 * Parameters: [drive, tone]
 * - drive: 0-1 (controls pre-gain from 1x to 12x)
 * - tone: 0-1 (LPF cutoff from 400Hz to 8200Hz)
 */
class SoftClipEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    float sample_rate_;
    float drive_;           // Pre-gain multiplier
    float tone_blend_old_;  // LPF coefficient: how much old to keep
    float tone_blend_new_;  // LPF coefficient: how much new to take
    float tone_memory_;     // Previous output for LPF
    
    static constexpr float OUTPUT_TRIM = 1.0f;
    static constexpr float OUTPUT_LIMIT = 1.2f;
};
