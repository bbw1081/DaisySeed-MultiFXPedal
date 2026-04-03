#pragma once

#include "BaseEffect.h"
#include <cmath>

/**
 * CMOS-style Distortion Effect (HSP_Protoseed inspired)
 * Odd-polynomial saturation with envelope sag
 * 
 * Parameters: [drive, crunch, fizz, pregain, sag, tone]
 * - drive: 0-1 (input gain)
 * - crunch: 0-1 (cubic non-linearity)
 * - fizz: 0-1 (fifth harmonic emphasis)
 * - pregain: 0-1 (pre-distortion gain)
 * - sag: 0-1 (envelope sag amount)
 * - tone: 0-1 (tone control, LPF cutoff)
 */
class CMOSEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    float sample_rate_;
    float drive_;
    float crunch_;
    float fizz_;
    float pregain_;
    float sag_;
    float sag_envelope_;
    float tone_blend_old_;
    float tone_blend_new_;
    float tone_memory_;
};
