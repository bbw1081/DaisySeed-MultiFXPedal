#pragma once

#include "BaseEffect.h"
#include <cmath>

/**
 * OpAmp-style Distortion Effect (HSP_Protoseed inspired)
 * Feedback clipper blended with diode hard clip
 * 
 * Parameters: [drive, soft_threshold, hard_threshold, mix, tone]
 * - drive: 0-1 (input gain)
 * - soft_threshold: 0-1 (soft clipper threshold)
 * - hard_threshold: 0-1 (hard clipper threshold)
 * - mix: 0-1 (soft->hard blend)
 * - tone: 0-1 (tone control, LPF cutoff)
 */
class OpAmpEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    float sample_rate_;
    float drive_;
    float soft_thresh_;
    float hard_thresh_;
    float mix_;
    float tone_blend_old_;
    float tone_blend_new_;
    float tone_memory_;
};
