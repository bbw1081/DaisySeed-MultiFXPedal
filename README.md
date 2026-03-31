# DaisySeed-MultiFXPedal
WORK IN PROGRESS - I am building a multiFX guitar pedal using the daisy seed platform, still very early development 

to get all dependencies make sure to run `git submodule update --init --recursive`

## PRESET FORMAT

the file naming convention is numbers starting from 1 in the order in which you want the presets to be on the pedal

example preset

```
const char preset[] = R"({
    "name": "overdrive wah",
    "effects": ["autowah", "overdrive"],
    "params": [[70, 0.8, 0.8], [0.7]]
})";
```

### Current Possible Effects

overdrive : [drive_amount (0-1)]

autowah : [dry_wet (0-100), level (0-1), wah (0-1)]

chorus : [delay (ms), feedback (0-1), lfo frequency (Hz)]

decimator : [bits to crush (0-16), downsample factor (float)]

flanger : [delay (ms), feedback (0-1), lfo frequency (Hz)]

phaser : [feedback (0-1), frequency (Hz), lfo frequency (Hz), poles (1-8)]

pitchshifter : [size (uint32_t), fun (float), transpose (semitones)]

tremolo : [depth (0-1), frequency (Hz)]

wavefolder : [gain (float), offset (float)]


Note: when adding new effects update effects/Effects.h, preset_management/preset.h & .cpp; when adding new presets update preset_management/PresetManager.cpp
