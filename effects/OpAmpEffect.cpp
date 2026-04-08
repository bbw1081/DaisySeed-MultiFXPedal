#include "OpAmpEffect.h"

void OpAmpEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // params[0] = drive (0-1) -> linear gain 1x to 12x
    drive_ = 1.0f + (params[0] * 11.0f);
    
    // params[1] = soft_threshold (0-1) -> soft clipper threshold
    soft_thresh_ = 0.2f + (params[1] * 0.4f);
    
    // params[2] = hard_threshold (0-1) -> hard clipper threshold
    hard_thresh_ = 0.3f + (params[2] * 0.5f);
    
    // params[3] = mix (0-1) -> soft<->hard blend
    mix_ = params[3];
    
    // params[4] = tone (0-1) -> cutoff frequency 400Hz to 8200Hz
    float tone_cutoff_hz = 400.0f + (params[4] * 7800.0f);
    
    // Initialize tone filter coefficients
    float tone_hz_clamped = fmaxf(10.0f, fminf(tone_cutoff_hz, sample_rate * 0.45f));
    float a = expf(-2.0f * 3.14159265f * tone_hz_clamped / sample_rate);
    tone_blend_old_ = a;
    tone_blend_new_ = 1.0f - a;
    
    tone_memory_ = 0.0f;
}

float OpAmpEffect::Process(float in) {
    // 1) Apply drive
    float driven = in * drive_ * 50.0f;
    
    // 2) Soft clipping using exponential saturation
    float soft;
    if (driven > soft_thresh_) {
        soft = soft_thresh_ + (1.0f - expf(-fminf(driven - soft_thresh_, 10.0f))) * 0.3f;
    } else if (driven < -soft_thresh_) {
        soft = -soft_thresh_ - (1.0f - expf(-fminf(-driven - soft_thresh_, 10.0f))) * 0.3f;
    } else {
        soft = driven;
    }
    
    // 3) Hard clipping at threshold
    float hard;
    if (driven > hard_thresh_) hard = hard_thresh_;
    else if (driven < -hard_thresh_) hard = -hard_thresh_;
    else hard = driven;
    
    // 4) Blend soft and hard clipping
    float mixed = soft * (1.0f - mix_) + hard * mix_;
    
    // 4) Tone control (one-pole low-pass filter)
    tone_memory_ = tone_blend_new_ * mixed + tone_blend_old_ * tone_memory_;
    float toned = tone_memory_;
    
    return toned;
}
