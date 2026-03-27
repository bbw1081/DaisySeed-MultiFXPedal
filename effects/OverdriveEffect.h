#pragma once

#include "BaseEffect.h"

/**
 * Overdrive Effect Class
 * 
 * @author Richard Wilkinson
 */
class OverdriveEffect : public BaseEffect {
public:

    /**
     * Initializes the Overdrive Effect
     * 
     * @param sample_rate audio sample rate
     * @param params array of effect parameters --> {drive}
     */
    void Init(float sample_rate, const float* params) override;

    /**
     * Process an audio sample
     */
    float Process(float in) override;

private:
    daisysp::Overdrive drive_;
};