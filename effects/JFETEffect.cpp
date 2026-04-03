#include "JFETEffect.h"

void JFETEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // params[0] = drive (0-1) -> linear gain 1x to 8x
    drive_ = 1.0f + (params[0] * 7.0f);
    
    // params[1] = grit (0-1) -> soft knee steepness
    grit_ = params[1] * 2.0f;
    
    // params[2] = bias (0-1) -> asymmetry
    bias_ = params[2];
    
    // params[3] = tone (0-1) -> cutoff frequency 400Hz to 8200Hz
    float tone_cutoff_hz = 400.0f + (params[3] * 7800.0f);
    
    // Initialize tone filter coefficients
    float tone_hz_clamped = fmaxf(10.0f, fminf(tone_cutoff_hz, sample_rate * 0.45f));
    float a = expf(-2.0f * 3.14159265f * tone_hz_clamped / sample_rate);
    tone_blend_old_ = a;
    tone_blend_new_ = 1.0f - a;
    
    tone_memory_ = 0.0f;
}

float JFETEffect::Process(float in) {
    // 1) Apply aggressive input pre-gain for strong distortion
    float boosted = in * 25.0f;
    
    // 2) Apply drive
    float driven = boosted * drive_;
    
    // 3) Bias offset
    float biased = driven + (bias_ - 0.5f) * 0.5f;
    
    // 4) JFET-style square-law saturation
    // Soft knee using: x / (1 + grit * x^2)
    float saturated;
    if (biased >= 0.0f) {
        float x2 = biased * biased;
        saturated = biased / (1.0f + grit_ * x2);
    } else {
        float x2 = biased * biased;
        saturated = biased / (1.0f + grit_ * x2);
    }
    
    // 5) Tone control (one-pole low-pass filter)
    tone_memory_ = tone_blend_new_ * saturated + tone_blend_old_ * tone_memory_;
    float toned = tone_memory_;
    
    return toned;
}
