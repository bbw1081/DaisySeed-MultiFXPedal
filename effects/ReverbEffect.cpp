#include "ReverbEffect.h"

static daisysp::ReverbSc DSY_SDRAM_BSS reverb_instance_;

void ReverbEffect::Init(float sample_rate, const float* params){
    reverb_ = &reverb_instance_;
    
    reverb_->Init(sample_rate);

    reverb_->SetFeedback(params[0]);
    reverb_->SetLpFreq(params[1]);

    outL_ = 0.0f;
    outR_ = 0.0f;
}

float ReverbEffect::Process(float in) { 
    reverb_->Process(in, in, &outL_, &outR_);

    return (outL_ + outR_) * 0.5f;
}