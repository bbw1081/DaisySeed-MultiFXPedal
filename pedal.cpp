/**
    pedal.cpp, main file for Daisy Seed MultiFX guitar pedal project
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
#include "pedal.h"

using namespace daisy;
using namespace daisysp;

//using 128x64 oled I2C display
using MyOledDisplay = OledDisplay<SSD130xI2c128x64Driver>;

/**
 * DAISY SEED PINOUT
 * A0     	<-- Potentiometer A10K ohms
 * A1     	<-- SPST Momentary Footswitch
 * A2		<-- Rotary Encoder DT
 * A3		<-- Rotary Encoder CLk
 * A4		<-- LED
 * A5		<-- Rotary Encoder Push Button
 * D11		<-- Display SCL
 * D12		<-- Display SDA
 * out[0] 	<-- 1/4" jack
 * in[0]  	<-- 1/4" jack
 */

DaisySeed hw; //daisy seed hardware
MyOledDisplay display;

//tuner state
#define TUNER_BUFFER_SIZE 2048
float tuner_buffer[TUNER_BUFFER_SIZE];
int tuner_buffer_index = 0;
bool tuner_buffer_ready = false;

enum PedalState { STATE_BYPASS, STATE_EFFECT, STATE_TUNER };
PedalState current_state = STATE_EFFECT;
PedalState pre_tuner = STATE_EFFECT;

//Note reference struct for tuner
struct NoteRef {
	const char* name;
	float freq;
};

const NoteRef NOTE_TABLE[] = {
    // E1 octave
    {"E1",  41.20f},  {"F1",  43.65f},  {"F#1", 46.25f},  {"G1",  49.00f},
    {"G#1", 51.91f},  {"A1",  55.00f},  {"A#1", 58.27f},  {"B1",  61.74f},
    {"C2",  65.41f},  {"C#2", 69.30f},  {"D2",  73.42f},  {"D#2", 77.78f},

    // E2 octave
    {"E2",  82.41f},  {"F2",  87.31f},  {"F#2", 92.50f},  {"G2",  98.00f},
    {"G#2", 103.83f}, {"A2",  110.00f}, {"A#2", 116.54f}, {"B2",  123.47f},
    {"C3",  130.81f}, {"C#3", 138.59f}, {"D3",  146.83f}, {"D#3", 155.56f},

    // E3 octave
    {"E3",  164.81f}, {"F3",  174.61f}, {"F#3", 185.00f}, {"G3",  196.00f},
    {"G#3", 207.65f}, {"A3",  220.00f}, {"A#3", 233.08f}, {"B3",  246.94f},
    {"C4",  261.63f}, {"C#4", 277.18f}, {"D4",  293.66f}, {"D#4", 311.13f},

    // E4 octave
    {"E4",  329.63f}, {"F4",  349.23f}, {"F#4", 369.99f}, {"G4",  392.00f},
    {"G#4", 415.30f}, {"A4",  440.00f}, {"A#4", 466.16f}, {"B4",  493.88f},
    {"C5",  523.25f}, {"C#5", 554.37f}, {"D5",  587.33f}, {"D#5", 622.25f},

    // E5 octave
    {"E5",  659.25f}, {"F5",  698.46f}, {"F#5", 739.99f}, {"G5",  783.99f},
    {"G#5", 830.61f}, {"A5",  880.00f}, {"A#5", 932.33f}, {"B5",  987.77f},
    {"C6",  1046.50f},{"C#6", 1108.73f},{"D6",  1174.66f},{"D#6", 1244.51f},
    {"E6",  1318.51f},
};
const int NOTE_TABLE_SIZE = sizeof(NOTE_TABLE) / sizeof(NOTE_TABLE[0]);

PresetManager preset_manager;

/**
 * Detects input pitch using autocorrelation
 */
float DetectPitch(float* buffer, int size, float sample_rate){
	int min_period = (int)(sample_rate / 1000.0f);
	int max_period = (int)(sample_rate / 35.0f);

	float best_correlation = -1.0f;
	float best_period = 0;

	for(int period = min_period; period < max_period && period < size / 2; period++){
		float correlation = 0.0f;
		float norm = 0.0f;

		for(int i = 0; i < size - period; i++){
			correlation += buffer[i] * buffer[i + period];
			norm += buffer[i] * buffer[i];
		}

		if(norm > 0.0f) correlation /= norm;

		if (correlation > best_correlation) {
			best_correlation = correlation;
			best_period = period;
		}
	}

	//weak correlation
	if (best_correlation < 0.3f || best_period == 0) return 0.0f;

	return sample_rate / (float)best_period;
}

/**
 * finds the nearest note and the offset in cents
 */
void FindNearestNote(float freq, const char** note_name, float* cents) {
	if(freq <= 0.0f) {
		*note_name = "--";
		*cents = 0.0f;
		return;
	}

	float best_distance = 1e9f;
	int best_index = 0;

	for (int i = 0; i < NOTE_TABLE_SIZE; i++) {
		float distance = fabsf(freq - NOTE_TABLE[i].freq);
		if (distance < best_distance) {
			best_distance = distance;
			best_index = i;
		}
	}

	*note_name = NOTE_TABLE[best_index].name;

	*cents = 1200.0f * log2f(freq / NOTE_TABLE[best_index].freq);
}

void UpdateTunerDisplay(const char* note, float cents) {
	display.Fill(false);

	//draw the border
	display.DrawRect(0, 0, 127, 63, true);

	//write note name
	display.WriteStringAligned(note, Font_11x18, Rectangle(0, 4, 128, 24), Alignment::centered, true);

	//tuning bars along bottom of screen
	const int NUM_BARS = 13;
	const int CENTER_BAR = 6;
	const int BAR_W = 7;
	const int BAR_GAP = 2;
	const int BAR_H = 16;
	const int BAR_Y = 44;
	const int BAR_X = 6 ;
	const float CENTS_PER_STEP = 5.0f;

	//determine active bar
	int active_bar = CENTER_BAR;
	if(strcmp(note, "--") != 0) {
		int offset = (int)(cents / CENTS_PER_STEP);
		active_bar = CENTER_BAR + offset;
		active_bar = std::max(0, std::min(NUM_BARS - 1, active_bar));
	}

	//draw the bars
	for(int i = 0; i < NUM_BARS; i++){
		int x = BAR_X + i * (BAR_W + BAR_GAP);
		bool filled = (i == active_bar);

		if(i == CENTER_BAR) {
			display.DrawRect(x, BAR_Y - 3, x + BAR_W, BAR_Y + BAR_H, true, filled);
		} else {
			display.DrawRect(x, BAR_Y, x + BAR_W, BAR_Y + BAR_H, true, filled);
		}
	}

	display.Update();
}

/**
 * Calculates the maximum font size based on the number of characters in a line
 */
FontDef CalculateFontSize(const char* text){
	if(strlen(text) < 11) {
		return Font_11x18;
	} else if(strlen(text) < 18) {
		return Font_7x10;
	} else {
		return Font_6x8;
	}
}

/**
 * Displays a string to the screen, can be 1 word or 2
 */
void DisplayText(const char* text){
	display.Fill(false); //clear the display
	display.DrawRect(0, 0, 127, 63, true); //draw a rectangle around the edge

	const char* spacePos = strchr(text, ' ');
	if(spacePos == nullptr) {
		//one line
		display.WriteStringAligned(text, CalculateFontSize(text), Rectangle(0, 0, 128, 64), Alignment::centered, true);
	} else {
		//two lines
		char line1[32];
		char line2[32];

		int i = spacePos - text;

		strncpy(line1, text, i);
		line1[i] = '\0';
		strcpy(line2, text + i + 1);

		display.WriteStringAligned(line1, CalculateFontSize(line1), Rectangle(0, 0, 128, 32), Alignment::centered, true);
		display.WriteStringAligned(line2, CalculateFontSize(line2), Rectangle(0, 32, 128, 32), Alignment::centered, true);
	}

    display.Update();
}

/**
 * Process the audio
 */
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
	float volume = hw.adc.Get(0) / 65535.0; //read volume pot

	for (size_t i = 0; i < size; i++)
	{
		if(current_state == STATE_BYPASS) {
			out[0][i] = in[0][i] * volume;
		} else if (current_state == STATE_EFFECT) {
			//effect
			out[0][i] = preset_manager.Process(in[0][i]) * volume;
		} else if (current_state == STATE_TUNER) {
			out[0][i] = 0;

			//fill the tuner buffer
			if(!tuner_buffer_ready){
				tuner_buffer[tuner_buffer_index++] = in[0][i];
				if(tuner_buffer_index >= TUNER_BUFFER_SIZE) {
					tuner_buffer_ready = true;
				}
			}
		}
	}
}

int main(void)
{
	//initialize hardware
	hw.Init();

	/** Configure the Display */
    MyOledDisplay::Config disp_cfg;

    disp_cfg.driver_config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    disp_cfg.driver_config.transport_config.i2c_config.mode   = I2CHandle::Config::Mode::I2C_MASTER;
    disp_cfg.driver_config.transport_config.i2c_config.speed  = I2CHandle::Config::Speed::I2C_400KHZ;
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = seed::D11;
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = seed::D12;

    display.Init(disp_cfg);    
    display.Fill(false);
    display.Update();
    System::Delay(100);

	/** initialize peripheral inputs and outputs **/
	const int num_adc_channels = 1; //number of adc channels in use
	AdcChannelConfig adc_config[num_adc_channels];

	//initialize volume pot connected to pin A0
	adc_config[0].InitSingle(seed::A0);
	hw.adc.Init(adc_config, num_adc_channels);
	hw.adc.Start();

	//initilaize SPST switch at pin A1
	GPIO switch_pin;
	switch_pin.Init(seed::A1, GPIO::Mode::INPUT, GPIO::Pull::PULLUP); 

	//initilaize rotary encoder inputs
	GPIO encoder_dt;
	GPIO encoder_clk;
	encoder_dt.Init(seed::A2, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
	encoder_clk.Init(seed::A3, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);

	GPIO encoder_switch;
	encoder_switch.Init(seed::A5, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);

	//init effect led
	GPIO led;
	led.Init(seed::A4, GPIO::Mode::OUTPUT);
	if(current_state == STATE_EFFECT){ led.Write(true); }

	/** initialize audio **/
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	//init the preset manager
	preset_manager.Init(hw.AudioSampleRate());

	//start audio
	hw.StartAudio(AudioCallback);

	/** set initial values **/
	DisplayText(preset_manager.GetName());

	bool encoder_switch_held = false;
	bool switch_held = false; //set switch held flag to 0
	bool prev_clock_state = true; //set rotary encoder default clock state

	while(1) {
		//tuner switch processing
		bool encoder_switch_state = encoder_switch.Read();
		if(!encoder_switch_state && !encoder_switch_held) {
			//change the state
			if(current_state != STATE_TUNER){
				current_state = STATE_TUNER;
			} else {
				current_state = pre_tuner;
			}

			encoder_switch_held = true;
		} else if (encoder_switch_state) {
			encoder_switch_held = false;
		}

		//footswitch processing
		bool switch_state = switch_pin.Read();
		if (!switch_state && !switch_held){
			//change state under the tuner
			if(pre_tuner == STATE_EFFECT) {
				pre_tuner = STATE_BYPASS;
			} else if(pre_tuner == STATE_BYPASS) {
				pre_tuner = STATE_EFFECT;
			}
			//if we are not currently using the tuner, go ahead and change the state
			if(current_state != STATE_TUNER) {
				current_state = pre_tuner;
			}

			switch_held = true;

			//change LED even under the tuner
			if(pre_tuner == STATE_BYPASS){
				led.Write(false);
			} else if (pre_tuner == STATE_EFFECT){
				led.Write(true);
			}
		} else if (switch_state){
			//if the switch is not being held down
			switch_held = false;
		}

		//rotary encoder processing, will not do anything if tuner is active
		if(current_state != STATE_TUNER) {
			bool current_clock_state = encoder_clk.Read();
			if (current_clock_state != prev_clock_state && !current_clock_state){
				bool current_dt_state = encoder_dt.Read();
				if (current_clock_state == current_dt_state) {
					preset_manager.ChangePreset(-1);
				} else {
					preset_manager.ChangePreset(1);
				}
			}
			prev_clock_state = current_clock_state;
		}

		//update the display
		if(current_state == STATE_TUNER) {
			float freq = DetectPitch(tuner_buffer, TUNER_BUFFER_SIZE, hw.AudioSampleRate());
			const char* note;
			float cents;
			FindNearestNote(freq, &note, &cents);
			UpdateTunerDisplay(note, cents);

			tuner_buffer_index = 0;
			tuner_buffer_ready = false;
		} else {
			DisplayText(preset_manager.GetName());
		}

		//delay the system, helps act as debounce
		System::Delay(20);
	}
}

