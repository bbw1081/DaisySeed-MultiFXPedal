#include "BJTEffect.h"

void BJTEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // params[0] = drive (0-1) -> linear gain 1x to 10x
    drive_ = 1.0f + (params[0] * 9.0f);
    
    // params[1] = steepness (0-1) -> clipping curve steepness
    steepness_ = params[1] * 3.0f;
    
    // params[2] = asymmetry (0-1) -> asymmetric clipping
    asymmetry_ = params[2];
    
    // params[3] = bias (0-1) -> bias voltage
    bias_ = params[3];
    
    // params[4] = tone (0-1) -> cutoff frequency 400Hz to 8200Hz
    float tone_cutoff_hz = 400.0f + (params[4] * 7800.0f);
    
    // Initialize tone filter coefficients
    float tone_hz_clamped = fmaxf(10.0f, fminf(tone_cutoff_hz, sample_rate * 0.45f));
    float a = expf(-2.0f * 3.14159265f * tone_hz_clamped / sample_rate);
    tone_blend_old_ = a;
    tone_blend_new_ = 1.0f - a;
    
    tone_memory_ = 0.0f;
}

float BJTEffect::Process(float in) {

    // 1) Apply drive
    float driven = in * drive_ * 50.0f;
    
    // 3) Diode-equation style soft clipping
    // Approximates: I = Is * (exp(V/Vt) - 1)
    // Creates strong odd harmonics characteristic of BJT circuits
    
    float biased = driven + bias_ - 0.5f;
    float clipped;
    
    // For positive half
    if (biased > 0.0f) {
        float exp_pos = expf(fminf(steepness_ * biased, 10.0f));
        clipped = (exp_pos - 1.0f) / (1.0f + steepness_) * 0.5f;
    }
    // For negative half - apply asymmetry
    else {
        float neg_steep = steepness_ * (0.5f + asymmetry_ * 0.5f);
        float exp_neg = expf(fminf(neg_steep * biased, 10.0f));
        clipped = (exp_neg - 1.0f) / (1.0f + neg_steep) * 0.5f;
    }
    
    // 3) Clamp to prevent blow-up
    clipped = fmaxf(-1.2f, fminf(1.2f, clipped));
    
    // 4) Tone control (one-pole low-pass filter)
    tone_memory_ = tone_blend_new_ * clipped + tone_blend_old_ * tone_memory_;
    float toned = tone_memory_;
    
    return toned * 0.8f;
}
