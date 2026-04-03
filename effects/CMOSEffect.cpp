#include "CMOSEffect.h"

void CMOSEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // params[0] = drive (0-1) -> linear gain 1x to 8x
    drive_ = 1.0f + (params[0] * 7.0f);
    
    // params[1] = crunch (0-1) -> cubic non-linearity
    crunch_ = params[1] * 1.5f;
    
    // params[2] = fizz (0-1) -> fifth harmonic emphasis
    fizz_ = params[2];
    
    // params[3] = pregain (0-1) -> pre-distortion gain 0.5x to 2x
    pregain_ = 0.5f + (params[3] * 1.5f);
    
    // params[4] = sag (0-1) -> envelope sag amount
    sag_ = params[4];
    
    // params[5] = tone (0-1) -> cutoff frequency 400Hz to 8200Hz
    float tone_cutoff_hz = 400.0f + (params[5] * 7800.0f);
    
    // Initialize tone filter coefficients
    float tone_hz_clamped = fmaxf(10.0f, fminf(tone_cutoff_hz, sample_rate * 0.45f));
    float a = expf(-2.0f * 3.14159265f * tone_hz_clamped / sample_rate);
    tone_blend_old_ = a;
    tone_blend_new_ = 1.0f - a;
    
    tone_memory_ = 0.0f;
    sag_envelope_ = 1.0f;
}

float CMOSEffect::Process(float in) {
    // 1) Apply aggressive input pre-gain for strong distortion
    float boosted = in * 25.0f;
    
    // 2) Apply pregain
    float pregained = boosted * pregain_;
    
    // 3) Apply drive
    float driven = pregained * drive_;
    
    // 4) Odd-polynomial distortion (3rd and 5th harmonics)
    // Creates harmonic-rich distortion characteristic of CMOS gates
    float x_clamp = fmaxf(-2.0f, fminf(2.0f, driven));
    
    // Cubic term (3rd harmonic - strong)
    float cubic = x_clamp * x_clamp * x_clamp * crunch_;
    
    // Fifth harmonic term (weaker)
    float fifth = x_clamp * x_clamp * x_clamp * x_clamp * x_clamp * fizz_ * 0.1f;
    
    // Combine and soft limit
    float distorted = x_clamp + cubic + fifth;
    distorted = distorted / (1.0f + fabsf(distorted) * 0.5f);
    
    // 5) Envelope sag (simplified: track magnitude)
    // High energy causes sag (volume loss)
    float magnitude = fabsf(distorted);
    float target_sag = 1.0f - (magnitude * sag_ * 0.5f);
    sag_envelope_ += 0.01f * (target_sag - sag_envelope_);
    
    float sagged = distorted * sag_envelope_;
    
    // 6) Tone control (one-pole low-pass filter)
    tone_memory_ = tone_blend_new_ * sagged + tone_blend_old_ * tone_memory_;
    float toned = tone_memory_;
    
    return toned * 0.8f;
}
