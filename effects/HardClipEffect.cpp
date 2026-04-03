#include "HardClipEffect.h"

void HardClipEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // params[0] = drive (0-1) -> linear gain 1x to 10x
    drive_ = 1.0f + (params[0] * 9.0f);
    
    // params[1] = tone (0-1) -> cutoff frequency 400Hz to 8200Hz
    tone_cutoff_hz_ = 400.0f + (params[1] * 7800.0f);
    
    // params[2] = symmetry (0-1) -> clipping asymmetry
    symmetry_ = params[2];
    
    // Initialize tone filter coefficients
    // One-pole LPF: y[n] = tone_blend_new * x[n] + tone_blend_old * y[n-1]
    // where tone_blend_old = exp(-2π*fc/fs), tone_blend_new = 1 - tone_blend_old
    float tone_hz_clamped = fmaxf(10.0f, fminf(tone_cutoff_hz_, sample_rate * 0.45f));
    float a = expf(-2.0f * 3.14159265f * tone_hz_clamped / sample_rate);
    tone_blend_old_ = a;
    tone_blend_new_ = 1.0f - a;
    
    tone_memory_ = 0.0f;
}

float HardClipEffect::Process(float in) {
    // 1) Apply aggressive input pre-gain for strong distortion
    float boosted = in * 25.0f;
    
    // 2) Apply drive (pre-gain)
    float driven = boosted * drive_;
    
    // 3) Hard clip with asymmetric thresholds
    // Symmetry parameter blends between soft lower clip and hard symmetric clip
    float upper_threshold = 0.8f + (0.2f * (1.0f - symmetry_));
    float lower_threshold = -0.8f - (0.2f * (1.0f - symmetry_));
    
    // Simple hard clipper
    float clipped = fmaxf(lower_threshold, fminf(upper_threshold, driven));
    
    // 4) Tone control (one-pole low-pass filter)
    // Reduces high frequencies (simulates amp tone cut)
    // y[n] = tone_blend_new * x[n] + tone_blend_old * y[n-1]
    tone_memory_ = tone_blend_new_ * clipped + tone_blend_old_ * tone_memory_;
    float toned = tone_memory_;
    
    // 5) Safety limiting to prevent digital clipping on extreme settings
    toned *= OUTPUT_TRIM;
    toned = fmaxf(-OUTPUT_LIMIT, fminf(OUTPUT_LIMIT, toned));
    
    return toned;
}
