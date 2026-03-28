# DaisySeed-MultiFXPedal
WORK IN PROGRESS - I am building a multiFX guitar pedal using the daisy seed platform, still very early development 

## PRESET FORMAT

the file naming convention is numbers starting from 1 in the order in which you want the presets to be on the pedal

example preset with overdrive into reverb

const char preset(num)[] = R"({
    "name": "name",
    "effects": ["overdrive", "reverb"],
    "params": [[overdrive params], [reverb params]]
})";


### Current Possible Effects

overdrive : [drive_amount]