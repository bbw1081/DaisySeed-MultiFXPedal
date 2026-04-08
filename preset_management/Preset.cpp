#include "Preset.h"

void Preset::Init(JsonDocument doc, int sample_rate){
    effect_count_ = 0;
    param_count_ = 0;

    // Always clear the chain first, otherwise a new preset can keep previous effects.
    effect_chain_.Clear();
    
    // Clear all parameters to ensure clean state
    for(int i = 0; i < MAX_EFFECTS; i++) {
        for(int j = 0; j < MAX_PARAMS; j++) {
            params_[i][j] = 0.0f;
        }
    }

    //parse name
    strcpy(name_, doc["name"].as<const char*>());
    
    //parse effects and parameters
    for (size_t i = 0; i < doc["effects"].size() && i < MAX_EFFECTS; ++i){
        strcpy(effects_[i], doc["effects"][i].as<const char*>());
        effect_count_++;

        for (size_t j = 0; j < doc["params"][i].size() && j < MAX_PARAMS; ++j) {
            params_[i][j] = doc["params"][i][j].as<float>();
            param_count_++;
        }
    }

    //add effect to chain based on the effect's name
    for (int i = 0; i < effect_count_; ++i) {
        if (strcmp(effects_[i], "overdrive") == 0) {
            od_->Init(sample_rate, params_[i]);
            effect_chain_.Add(od_);
        } else if (strcmp(effects_[i], "autowah") == 0) {
            wah_->Init(sample_rate, params_[i]);
            effect_chain_.Add(wah_);
        } else if (strcmp(effects_[i], "chorus") == 0) {
            chorus_->Init(sample_rate, params_[i]);
            effect_chain_.Add(chorus_);
        } else if (strcmp(effects_[i], "decimator") == 0) {
            decimator_->Init(sample_rate, params_[i]);
            effect_chain_.Add(decimator_);
        } else if (strcmp(effects_[i], "eq") == 0) {
            eq_->Init(sample_rate, params_[i]);
            effect_chain_.Add(eq_);
        } else if (strcmp(effects_[i], "flanger") == 0) {
            flanger_->Init(sample_rate, params_[i]);
            effect_chain_.Add(flanger_);
        } else if (strcmp(effects_[i], "phaser") == 0 ) {
            phaser_->Init(sample_rate, params_[i]);
            effect_chain_.Add(phaser_);
        } else if (strcmp(effects_[i], "pitchshifter") == 0) {
            pitch_->Init(sample_rate, params_[i]);
            effect_chain_.Add(pitch_);
        } else if (strcmp(effects_[i], "tremolo") == 0) {
            trem_->Init(sample_rate, params_[i]);
            effect_chain_.Add(trem_);
        } else if (strcmp(effects_[i], "wavefolder") == 0) {
            folder_->Init(sample_rate, params_[i]);
            effect_chain_.Add(folder_);
        } else if (strcmp(effects_[i], "hard_clip") == 0) {
            hard_clip_->Init(sample_rate, params_[i]);
            effect_chain_.Add(hard_clip_);
        } else if (strcmp(effects_[i], "soft_clip") == 0) {
            soft_clip_->Init(sample_rate, params_[i]);
            effect_chain_.Add(soft_clip_);
        } else if (strcmp(effects_[i], "jfet") == 0) {
            jfet_->Init(sample_rate, params_[i]);
            effect_chain_.Add(jfet_);
        } else if (strcmp(effects_[i], "bjt") == 0) {
            bjt_->Init(sample_rate, params_[i]);
            effect_chain_.Add(bjt_);
        } else if (strcmp(effects_[i], "opamp") == 0) {
            opamp_->Init(sample_rate, params_[i]);
            effect_chain_.Add(opamp_);
        } else if (strcmp(effects_[i], "cmos") == 0) {
            cmos_->Init(sample_rate, params_[i]);
            effect_chain_.Add(cmos_);
        }
    }
}

float Preset::Process(float in) { return effect_chain_.Process(in); }

const char* Preset::GetName() { return name_; }