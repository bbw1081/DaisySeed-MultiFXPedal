/**
    PresetManager.cpp, contains a class that manages presets and switching between them
    Copyright (C) 2026  Richard Wilkinson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "PresetManager.h"

void PresetManager::Init(int sample_rate){
    sample_rate_ = sample_rate;
    num_presets_ = 0;

    // Preset array init
    preset_data_[0] = preset1;
    preset_data_[1] = preset2;
    preset_data_[2] = preset3;
    preset_data_[3] = preset4;
    preset_data_[4] = preset5;
    preset_data_[5] = preset6;
    preset_data_[6] = preset7;
    preset_data_[7] = preset8;
    preset_data_[8] = preset9;
    preset_data_[9] = preset10;

    num_presets_ = 10;
    SetActivePreset(1);
}

void PresetManager::SetActivePreset(int val) {
    if(val > 0 && val <= num_presets_) {
        current_preset_num_ = val;

        // Set the pointers to shared effect instances in the preset
        current_preset_.wah_ = &wah_;
        current_preset_.bjt_ = &bjt_;
        current_preset_.chorus_ = &chorus_;
        current_preset_.cmos_ = &cmos_;
        current_preset_.decimator_ = &decimator_;
        current_preset_.eq_ = &eq_;
        current_preset_.flanger_ = &flanger_;
        current_preset_.hard_clip_ = &hard_clip_;
        current_preset_.jfet_ = &jfet_;
        current_preset_.opamp_ = &opamp_;
        current_preset_.od_ = &od_;
        current_preset_.phaser_ = &phaser_;
        current_preset_.pitch_ = &pitch_;
        current_preset_.soft_clip_ = &soft_clip_;
        current_preset_.trem_ = &trem_;
        current_preset_.folder_ = &folder_;
        current_preset_.reverb_ = &reverb_;
        current_preset_.delay_ = &delay_;
        current_preset_.revdelay_ = &revdelay_;

        JsonDocument doc;
        deserializeJson(doc, preset_data_[val - 1]);

        current_preset_.Init(doc, sample_rate_);
    }
}

void PresetManager::ChangePreset(int val){
    //either increment or decrement the preset, if val is more than 1 (somehow) will not do anything
    if (val == 1){
        if (current_preset_num_ == num_presets_) {
            SetActivePreset(1);
        } else {
            SetActivePreset(current_preset_num_ + 1);
        }
    } else if (val == -1){
        if (current_preset_num_ == 1) {
            SetActivePreset(num_presets_);
        } else {
            SetActivePreset(current_preset_num_ - 1);
        }
    }
}

float PresetManager::Process(float in) { return current_preset_.Process(in); }

const char* PresetManager::GetName() { return current_preset_.GetName(); }