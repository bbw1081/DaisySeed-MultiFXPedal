#include "Preset.h"

void Preset::Init(JsonDocument doc, int sample_rate){
    effect_count_ = 0;
    param_count_ = 0;

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

    for (int i = 0; i < effect_count_; ++i) {
        if (strcmp(effects_[i], "overdrive") == 0) {
            od_.Init(sample_rate, params_[i]);
            effect_chain_.Add(&od_);
        }
    }
}

float Preset::Process(float in) {
    return effect_chain_.Process(in);
}