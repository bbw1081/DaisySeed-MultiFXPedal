#pragma once

#include "BaseEffect.h"
#include <cmath>

/**
 * BJT-style Distortion Effect (HSP_Protoseed inspired)
 * Diode-equation style soft clip with strong odd harmonics
 * 
 * Parameters: [drive, steepness, asymmetry, bias, tone]
 * - drive: 0-1 (input gain)
 * - steepness: 0-1 (clipping curve steepness)
 * - asymmetry: 0-1 (asymmetric clipping)
 * - bias: 0-1 (bias voltage)
 * - tone: 0-1 (tone control, LPF cutoff)
 */
class BJTEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    float sample_rate_;
    float drive_;
    float steepness_;
    float asymmetry_;
    float bias_;
    float tone_blend_old_;
    float tone_blend_new_;
    float tone_memory_;
};
