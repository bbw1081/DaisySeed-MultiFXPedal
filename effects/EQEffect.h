#pragma once

#include "BaseEffect.h"

/**
 * 10-Band Parametric EQ Effect
 * 
 * Parameters (10 values for gain of each band in dB, 0-1 range maps to -12 to +12 dB):
 * - params[0]: 31 Hz band gain
 * - params[1]: 62 Hz band gain
 * - params[2]: 125 Hz band gain
 * - params[3]: 250 Hz band gain
 * - params[4]: 500 Hz band gain
 * - params[5]: 1 kHz band gain
 * - params[6]: 2 kHz band gain
 * - params[7]: 4 kHz band gain
 * - params[8]: 8 kHz band gain
 * - params[9]: 16 kHz band gain
 */
class EQEffect : public BaseEffect {
public:
    void Init(float sample_rate, const float* params) override;
    float Process(float in) override;

private:
    static constexpr int NUM_BANDS = 10;
    static constexpr float CENTER_FREQUENCIES[NUM_BANDS] = {
        31.0f, 62.0f, 125.0f, 250.0f, 500.0f,
        1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f
    };
    
    // State Variable Filter for each band (peaking mode)
    daisysp::Svf filters_[NUM_BANDS];
    float gains_[NUM_BANDS];
    float sample_rate_;
};
