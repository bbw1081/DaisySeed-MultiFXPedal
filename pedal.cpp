#include "pedal.h"

using namespace daisy;
using namespace daisysp;

/**
 * DAISY SEED PINOUT
 * A0     	<-- Potentiometer A10K ohms
 * A1     	<-- SPST Momentary Footswitch
 * A2		<-- Rotary Encoder DT
 * A3		<-- Rotary Encoder CLk
 * out[0] 	<-- 1/4" jack
 * in[0]  	<-- 1/4" jack
 */

DaisySeed hw; //daisy seed hardware
bool bypass = true; //bypass flag

PresetManager preset_manager;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float volume = hw.adc.Get(0) / 65535.0; //read volume pot

	for (size_t i = 0; i < size; i++)
	{
		if(bypass) {
			out[0][i] = in[0][i] * volume;
		} else {
			//effect
			out[0][i] = preset_manager.Process(in[0][i]) * volume;
		}
	}
}

int main(void)
{
	//initialize hardware
	hw.Init();

	//start hardware logging, used for debugging over serial output
	hw.StartLog();

	//initialize ADC
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


	//initialize audio
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	//init the preset manager
	preset_manager.Init(hw.AudioSampleRate());

	//start audio
	hw.StartAudio(AudioCallback);

	//set switch held flag to 0
	bool switch_held = false; 
	bool prev_clock_state = true;

	while(1) {
		//switch processing
		bool switch_state = switch_pin.Read();
		if (!switch_state && !switch_held){
			bypass = !bypass;
			switch_held = true;
		} else if (switch_state){
			switch_held = false;
		}

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

		//delay the system, helps act as debounce
		System::Delay(10);
	}
}

