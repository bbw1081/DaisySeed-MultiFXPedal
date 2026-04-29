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

    for(int i = 1; i <= MAX_PRESETS; i++){
        char path[MAX_FILENAME];
        snprintf(path, sizeof(path), "/presets/%d.json", i);

        FIL f;
        FRESULT res = f_open(&f, path, FA_READ);
        if(res != FR_OK) break; //stop at first missing file
        f_close(&f);

        snprintf(preset_filenames_[num_presets_], MAX_FILENAME, "/presets/%d.json", i);
        num_presets_++;
    }

    if(num_presets_ > 0) SetActivePreset(1);
}

void PresetManager::SetActivePreset(int val) {
    if(val > 0 && val <= num_presets_) {
        current_preset_num_ = val;

        FIL f;
        FRESULT res = f_open(&f, preset_filenames_[val-1], FA_READ);
        if(res != FR_OK) return; //file open failed

        UINT bytes_read;
        f_read(&f, file_buf_, sizeof(file_buf_) - 1, &bytes_read);
        f_close(&f);
        file_buf_[bytes_read] = '\0'; //null terminate

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
        deserializeJson(doc, file_buf_);
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

int PresetManager::GetNumPresets() { return num_presets_; }