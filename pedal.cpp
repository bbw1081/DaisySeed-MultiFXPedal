#include "pedal.h"

using namespace daisy;
using namespace daisysp;

/**
 * DAISY SEED PINOUT
 * A0     	<-- Potentiometer A10K ohms
 * A1     	<-- SPST Momentary Footswitch
 * A2		<-- SPST Momentary Footswitch
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
	const int num_adc_channels = 3; //number of adc channels in use
	AdcChannelConfig adc_config[num_adc_channels];
	adc_config[0].InitSingle(seed::A0); //volume potentiometer connected to pin A0
	GPIO switch_pin;
	GPIO switch_pin_2;
	switch_pin.Init(seed::A1, GPIO::Mode::INPUT, GPIO::Pull::PULLUP); //initilaize footswitch pin at A1
	switch_pin_2.Init(seed::A2, GPIO::Mode::INPUT, GPIO::Pull::PULLUP); //initilaize footswitch pin at A2
	hw.adc.Init(adc_config, num_adc_channels);
	hw.adc.Start();

	//initialize audio
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	preset_manager.Init(hw.AudioSampleRate());

	hw.StartAudio(AudioCallback);

	bool switch_held = false; //set switch held flag to 0
	bool switch_held_2 = false;

	while(1) {
		//footswitch processing
		bool switch_state = switch_pin.Read();
		if (!switch_state && !switch_held){
			bypass = !bypass;
			switch_held = true;
		} else if (switch_state){
			switch_held = false;
		}

		//footswitch 2 processing
		bool switch_state_2 = switch_pin_2.Read();
		if (!switch_state_2 && !switch_held_2){
			preset_manager.ChangePreset(1);
			switch_held_2 = true;
		} else if (switch_state_2){
			switch_held_2 = false;
		}

		//delay the system, helps act as footswitch debounce
		System::Delay(10);
	}
}

