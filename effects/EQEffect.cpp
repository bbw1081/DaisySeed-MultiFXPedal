#include "EQEffect.h"
#include <math.h>

// Define static array
constexpr const float EQEffect::CENTER_FREQUENCIES[10];

void EQEffect::Init(float sample_rate, const float* params) {
    sample_rate_ = sample_rate;
    
    // Initialize all bands with default gain of 0 dB (no change)
    for (int i = 0; i < NUM_BANDS; i++) {
        filters_[i].Init(sample_rate);
        filters_[i].SetFreq(CENTER_FREQUENCIES[i]);
        filters_[i].SetRes(0.707f);  // Q factor for smooth response
        filters_[i].SetDrive(1.0f);
        gains_[i] = 0.0f;
    }
    
    // Set initial gains from parameters if provided
    if (params != nullptr) {
        for (int i = 0; i < NUM_BANDS; i++) {
            // Map parameter range 0-1 to gain range -12 to +12 dB
            gains_[i] = (params[i] - 0.5f) * 24.0f;
        }
    }
}

float EQEffect::Process(float in) {
    float out = in;
    
    // Process through all EQ bands and mix
    for (int i = 0; i < NUM_BANDS; i++) {
        filters_[i].Process(in);
        
        // Get the peak (peaking filter) output
        float peak_out = filters_[i].Peak();
        
        // Convert gain from dB to linear
        float gain_linear = powf(10.0f, gains_[i] / 20.0f);
        
        // Add the boosted/cut band signal to the output
        // When gain_linear = 1 (0 dB), add nothing (peak_out * 0)
        // When gain_linear > 1 (boost), add the boosted peak signal
        // When gain_linear < 1 (cut), subtract the band
        out += peak_out * (gain_linear - 1.0f);
    }
    
    return out;
}
