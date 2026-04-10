#include "DelayEffect.h"

//max delay currently set to 1 second, increase value for longer delay times
static daisysp::DelayLine<float, 48000> DSY_SDRAM_BSS delay_instance_;

void DelayEffect::Init(float sample_rate, const float* params){
    delay_ = &delay_instance_;
    sample_rate_ = sample_rate;

    delay_->Init();

    float samples = params[0] * 48.0f;
    delay_->SetDelay(samples);

    feedback_ = params[1];
    mix_ = params[2];
}

float DelayEffect::Process(float in){
    float delayed = delay_->Read();

    delay_->Write(in + delayed * feedback_);

    return (in * (1.0f - mix_)) + (delayed * mix_);
}