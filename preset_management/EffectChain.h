#include "../effects/Effects.h"

static const int MAX_EFFECTS = 4;

class EffectChain {
public:
    void Clear() { count_ = 0; }

    void Add(BaseEffect* effect) {
        if (count_ < MAX_EFFECTS) {
            effects_[count_] = effect;
            count_++;
        }
    }

    float Process(float in) {
        float sample = in;
        for (int i = 0; i < count_; i++) {
            sample = effects_[i]->Process(sample);
        }
        return sample;
    }

private:
    BaseEffect* effects_[MAX_EFFECTS] = {};
    int count_ = 0;
};