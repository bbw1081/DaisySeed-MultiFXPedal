#pragma once

#include "BaseEffect.h"
#include <cmath>

/**
 * Hard Clipping Distortion Effect (HSP_Protoseed inspired)
 * Asymmetric hard clipping with tone control
 * 
 * Parameters: [drive, tone, symmetry]
 * - drive: 0-1 (controls pre-gain from 1x to 10x)
 * - tone: 0-1 (LPF cutoff from 400Hz to 8200Hz)
 * - symmetry: 0-1 (0=soft lower clip, 1=hard symmetric)
 */
class HardClipEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    float sample_rate_;
    float drive_;           // Pre-gain multiplier
    float tone_cutoff_hz_;  // Tone control cutoff frequency
    float tone_blend_old_;  // LPF coefficient: how much old to keep
    float tone_blend_new_;  // LPF coefficient: how much new to take
    float tone_memory_;     // Previous output for LPF
    float symmetry_;        // Clipping asymmetry (0=soft lower, 1=hard)
    
    static constexpr float OUTPUT_TRIM = 1.0f;
    static constexpr float OUTPUT_LIMIT = 1.2f;
};
