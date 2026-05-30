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
#include <cstdio>
#include <cstring>

PresetManager::PresetManager() 
    : num_presets_(0), sample_rate_(0), current_preset_num_(0), last_error_(PRESET_ERR_NONE) {
    // Initialize all arrays
    memset(preset_filenames_, 0, sizeof(preset_filenames_));
    memset(file_buf_, 0, sizeof(file_buf_));
    memset(error_msg_, 0, sizeof(error_msg_));
}

void PresetManager::Init(int sample_rate){
    sample_rate_ = sample_rate;
    num_presets_ = 0;
    last_error_ = PRESET_ERR_SD_INIT_FAIL;
    strcpy(error_msg_, "INIT ERROR");

    // Try to open a root file first to test if filesystem is responding
    FIL root_test;
    FRESULT root_res = f_open(&root_test, "/README.md", FA_READ);
    
    if(root_res == FR_OK) {
        f_close(&root_test);
        strcpy(error_msg_, "ROOT OK");
        return;  // Stop here to see this message
    } else {
        // Root file open failed - show the error code
        last_error_ = PRESET_ERR_FS_ERROR;
        snprintf(error_msg_, MAX_ERROR_MSG, "ROOT ERR %d", (int)root_res);
        return;
    }
}

void PresetManager::SetActivePreset(int val) {
    if(val <= 0 || val > num_presets_) {
        last_error_ = PRESET_ERR_JSON_PARSE;
        strcpy(error_msg_, "BAD PRESET #");
        return;
    }

    current_preset_num_ = val;

    FIL f;
    FRESULT res = f_open(&f, preset_filenames_[val-1], FA_READ);
    if(res != FR_OK) {
        last_error_ = PRESET_ERR_FILE_OPEN;
        strcpy(error_msg_, "OPEN FAIL");
        return;
    }

    // Get file size using f_size()
    FSIZE_t file_size = f_size(&f);
    
    // Sanity checks on file size
    if(file_size == 0) {
        f_close(&f);
        last_error_ = PRESET_ERR_FILE_READ;
        strcpy(error_msg_, "SIZE ZERO");
        return;
    }

    if(file_size > 4096) {  // Reasonable max for JSON preset
        f_close(&f);
        last_error_ = PRESET_ERR_FILE_READ;
        strcpy(error_msg_, "SIZE BIG");
        return;
    }

    // Seek to start
    res = f_lseek(&f, 0);
    if(res != FR_OK) {
        f_close(&f);
        last_error_ = PRESET_ERR_FILE_READ;
        strcpy(error_msg_, "SEEK FAIL");
        return;
    }

    // Clear buffer before reading
    memset(file_buf_, 0, sizeof(file_buf_));

    // Try reading byte by byte to debug
    UINT total_read = 0;
    for(UINT i = 0; i < file_size; i++) {
        UINT br = 0;
        res = f_read(&f, (uint8_t*)&file_buf_[i], 1, &br);
        if(res != FR_OK || br != 1) {
            f_close(&f);
            last_error_ = PRESET_ERR_FILE_READ;
            strcpy(error_msg_, "BYTE READ ERR");
            return;
        }
        total_read++;
    }
    
    f_close(&f);
    
    if(total_read != file_size) {
        last_error_ = PRESET_ERR_FILE_READ;
        strcpy(error_msg_, "PARTIAL READ");
        return;
    }

    file_buf_[total_read] = '\0'; // null terminate

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
    DeserializationError err = deserializeJson(doc, file_buf_);
    if(err) {
        last_error_ = PRESET_ERR_JSON_PARSE;
        strcpy(error_msg_, "JSON PARSE ERR");
        return;
    }

    // Check if required fields exist
    if(!doc["effects"] || !doc["params"]) {
        last_error_ = PRESET_ERR_JSON_PARSE;
        strcpy(error_msg_, "BAD JSON DATA");
        return;
    }

    last_error_ = PRESET_ERR_NONE;
    memset(error_msg_, 0, MAX_ERROR_MSG);
    current_preset_.Init(doc, sample_rate_);
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

PresetError PresetManager::GetLastError() { return last_error_; }

const char* PresetManager::GetLastErrorMsg() { return error_msg_; }