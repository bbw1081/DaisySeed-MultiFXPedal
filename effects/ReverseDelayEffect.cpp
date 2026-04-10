#include "ReverseDelayEffect.h"

static daisysp::DelayLineReverse<float, 48000> DSY_SDRAM_BSS delay_instance_;

void ReverseDelayEffect::Init(float sample_rate, const float* params){
    sample_rate_ = sample_rate;
    float delay_time = params[0];
    feedback_ = params[1];
    mix_ = params[2];

    delay_ = &delay_instance_;

    delay_->Init();
    delay_->SetDelay1(delay_time * 48.0f);
}

float ReverseDelayEffect::Process(float in){
    float delayed = delay_->ReadRev();
    delay_->Write(in + delayed * feedback_);
    return (in * (1.0f - mix_) + (delayed * mix_));
}