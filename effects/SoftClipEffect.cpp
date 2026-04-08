#include "SoftClipEffect.h"

void SoftClipEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // params[0] = drive (0-1) -> linear gain 1x to 12x
    drive_ = 1.0f + (params[0] * 11.0f);
    
    // params[1] = tone (0-1) -> cutoff frequency 400Hz to 8200Hz
    float tone_cutoff_hz = 400.0f + (params[1] * 7800.0f);
    
    // Initialize tone filter coefficients
    float tone_hz_clamped = fmaxf(10.0f, fminf(tone_cutoff_hz, sample_rate * 0.45f));
    float a = expf(-2.0f * 3.14159265f * tone_hz_clamped / sample_rate);
    tone_blend_old_ = a;
    tone_blend_new_ = 1.0f - a;
    
    tone_memory_ = 0.0f;
}

float SoftClipEffect::Process(float in) {
    // 1) Apply aggressive input pre-gain for strong distortion
    float boosted = in * 50.0f;
    
    // 2) Apply drive (pre-gain)
    float driven = boosted * drive_;
    
    // 3) Soft clipping using tanh
    // Creates smooth, musical saturation without hard edges
    float clipped = tanhf(driven);
    
    // 4) Tone control (one-pole low-pass filter)
    tone_memory_ = tone_blend_new_ * clipped + tone_blend_old_ * tone_memory_;
    float toned = tone_memory_;
    
    // 5) Safety limiting to prevent digital clipping
    toned *= OUTPUT_TRIM;
    toned = fmaxf(-OUTPUT_LIMIT, fminf(OUTPUT_LIMIT, toned));
    
    return toned;
}
