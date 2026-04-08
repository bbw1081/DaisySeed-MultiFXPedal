#include "../effects/Effects.h"

static const int MAX_EFFECTS = 4;

/**
 * Class to represent the effect chain
 * 
 * @author Richard Wilkinson
 */
class EffectChain {
public:
    /**
     * Clear the effect chain
     */
    void Clear() { count_ = 0; }

    /**
     * Add an effect to the chain
     * 
     * @param effect the effect to add to the chain
     */
    void Add(BaseEffect* effect) {
        if (count_ < MAX_EFFECTS) {
            effects_[count_] = effect;
            count_++;
        }
    }

    /**
     * Process the effect chain
     * 
     * @param in audio sample input
     * 
     * @return the processed audio sample
     */
    float Process(float in) {
        float sample = in;
        for (int i = 0; i < count_; i++) {
            sample = effects_[i]->Process(sample);
        }
        return sample;
    }

private:
    //array of effects in the chain
    BaseEffect* effects_[MAX_EFFECTS] = {};

    //number of effects currently in the chain
    int count_ = 0;
};