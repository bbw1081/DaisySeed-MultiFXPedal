#pragma once
#include <stddef.h>

/**
 * Interface class for all effect types
 * 
 * @author Richard Wilkinson
 */
class BaseEffect {
public:

    virtual ~BaseEffect() {}

    virtual void Init(float sample_rate, const float* params) = 0;

    virtual float Process(float in) = 0;
};