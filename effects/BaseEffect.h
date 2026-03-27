#pragma once

#include "daisysp.h"
#include <stddef.h>

/**
 * Interface class for all effect types
 * 
 * @author Richard Wilkinson
 */
class BaseEffect {
public:

    /**
     * Base Effect Destructor
     */
    virtual ~BaseEffect() {}

    /**
     * Initialize the effect
     * 
     * @param sample_rate audio sample rate as a float
     * @param params array of effect parameters, will vary by effect
     */
    virtual void Init(float sample_rate, const float* params) = 0;

    virtual float Process(float in) = 0;
};