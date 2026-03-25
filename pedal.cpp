#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float volume = hw.adc.Get(0) / 65535.0;

	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i] * volume;
		out[1][i] = in[1][i]  * volume;
	}
}

int main(void)
{
	//initialize hardware
	hw.Init();

	//start hardware logging, used for debugging over serial output
	hw.StartLog();

	//initialize potentiometer adc
	AdcChannelConfig adcConfig;
	adcConfig.InitSingle(seed::A0); //volume potentiometer connected to pin A0
	hw.adc.Init(&adcConfig, 1);
	hw.adc.Start();

	//initialize audio
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	while(1) {}
}

