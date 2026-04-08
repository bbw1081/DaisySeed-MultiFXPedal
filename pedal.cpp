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
 * D11		<-- Display SCL
 * D12		<-- Display SDA
 * out[0] 	<-- 1/4" jack
 * in[0]  	<-- 1/4" jack
 */

DaisySeed hw; //daisy seed hardware
MyOledDisplay display;

bool bypass = false; //bypass flag

PresetManager preset_manager;

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

	//init effect led
	GPIO led;
	led.Init(seed::A4, GPIO::Mode::OUTPUT);
	if(!bypass){ led.Write(true); }

	/** initialize audio **/
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	//init the preset manager
	preset_manager.Init(hw.AudioSampleRate());

	//start audio
	hw.StartAudio(AudioCallback);

	/** set initial values **/

	DisplayText(preset_manager.GetName());

	bool switch_held = false; //set switch held flag to 0
	bool prev_clock_state = true; //set rotary encoder default clock state

	while(1) {
		//switch processing
		bool switch_state = switch_pin.Read();
		if (!switch_state && !switch_held){
			bypass = !bypass;
			switch_held = true;
			if(bypass){
				led.Write(false);
			} else {
				led.Write(true);
			}
			DisplayText(preset_manager.GetName());
		} else if (switch_state){
			switch_held = false;
		}

		//rotary encoder processing
		bool current_clock_state = encoder_clk.Read();
		if (current_clock_state != prev_clock_state && !current_clock_state){
			bool current_dt_state = encoder_dt.Read();
			if (current_clock_state == current_dt_state) {
				preset_manager.ChangePreset(-1);
			} else {
				preset_manager.ChangePreset(1);
			}
			//update the display
			DisplayText(preset_manager.GetName());
		}
		prev_clock_state = current_clock_state;

		//delay the system, helps act as debounce
		System::Delay(50);
	}
}

