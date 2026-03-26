#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw; //daisy seed hardware
bool bypass = true; //bypass flag

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float volume = hw.adc.Get(0) / 65535.0; //read volume pot

	for (size_t i = 0; i < size; i++)
	{
		if(bypass) {
			out[0][i] = in[0][i] * volume;
			out[1][i] = in[1][i]  * volume;
		} else {
			//effect, currently mutes
			out[0][i] = in[0][i] * 0;
			out[1][i] = in[1][i]  * 0;
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
	const int num_adc_channels = 2; //number of adc channels in use
	AdcChannelConfig adc_config[num_adc_channels];
	adc_config[0].InitSingle(seed::A0); //volume potentiometer connected to pin A0
	GPIO switch_pin;
	switch_pin.Init(seed::A1, GPIO::Mode::INPUT, GPIO::Pull::PULLUP); //initilaize footswitch pin at A1
	hw.adc.Init(adc_config, num_adc_channels);
	hw.adc.Start();

	//initialize audio
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	bool switch_held = false; //set switch held flag to 0

	while(1) {
		//footswitch processing
		bool switch_state = switch_pin.Read();
		if (!switch_state && !switch_held){
			bypass = !bypass;
			switch_held = true;
		} else if (switch_state){
			switch_held = false;
		}

		//delay the system, helps act as footswitch debounce
		System::Delay(10);
	}
}

