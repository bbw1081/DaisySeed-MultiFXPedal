# DaisySeed-MultiFXPedal
A multi-effects guitar pedal platform built on the daisy seed hardware. Still in active development so expect changes 

## DEPENDENCIES

to get all dependencies make sure to run `git submodule update --init --recursive`

## CREDITS

Reverse Delay effect: https://github.com/AdamFulford/Veno-Echo/blob/main/Veno-Echo/delayline_reverse.h

Distortion Effects: https://github.com/Harold-Street-Pedal-Company/HSP_Protoseed

While I haven't directly used any code from this repository, it has given me inspiration for both the architecture and effects of my project:

https://github.com/bkshepherd/DaisySeedProjects

## PRESET FORMAT

The file naming convention is numbers starting from 1 in the order in which you want the presets to be on the pedal

Example preset:

```
const char preset[] = R"({
    "name": "overdrive wah",
    "effects": ["autowah", "overdrive"],
    "params": [[70, 0.8, 0.8], [0.7]]
})";
```

Note: when adding or removing presets please update PresetManager.cpp

## Current Effects

autowah : [dry_wet (0-100), level (0-1), wah (0-1)]

chorus : [delay (ms), feedback (0-1), lfo frequency (Hz)]

decimator : [bits to crush (0-16), downsample factor (float)]

delay : [delay time (ms, up to 1000), feedback (0-1), mix (0-1)]

eq : [31Hz (0-1), 62Hz (0-1), 125Hz (0-1), 250Hz (0-1), 500Hz (0-1), 1kHz (0-1), 2kHz (0-1), 4kHz (0-1), 8kHz (0-1), 16kHz (0-1)] - 10-band parametric EQ (0.5 = no change, -12dB to +12dB range)

flanger : [delay (ms), feedback (0-1), lfo frequency (Hz)]

overdrive : [drive_amount (0-1)]

phaser : [feedback (0-1), frequency (Hz), lfo frequency (Hz), poles (1-8)]

pitchshifter : [delay size (uint32_t), fun (float), transpose (semitones), mix (0-1)]

reverb : [feedback (0-1), LpFrequency (0 - 24kHz)]

reversedelay : (delay time (ms), feedback (0-1), mix (0-1)) /** from https://github.com/AdamFulford/Veno-Echo/blob/main/Veno-Echo/delayline_reverse.h **/

tremolo : [depth (0-1), frequency (Hz)]

wavefolder : [gain (float), offset (float)]

/** Distortion Effects: Inspired by https://github.com/Harold-Street-Pedal-Company/HSP_Protoseed **/

hard_clip : [drive (0-1 -> 1x-10x gain), tone (0-1 -> 400Hz-8200Hz LPF), symmetry (0-1 -> soft to hard clipping)]

soft_clip : [drive (0-1 -> 1x-12x gain), tone (0-1 -> 400Hz-8200Hz LPF)]

jfet : [drive (0-1 -> 1x-8x gain), grit (0-1 -> soft knee steepness), bias (0-1 -> asymmetry), tone (0-1 -> 400Hz-8200Hz LPF)]

bjt : [drive (0-1 -> 1x-10x gain), steepness (0-1 -> curve steepness), asymmetry (0-1), bias (0-1), tone (0-1 -> 400Hz-8200Hz LPF)]

opamp : [drive (0-1 -> 1x-12x gain), soft_threshold (0-1), hard_threshold (0-1), mix (0-1 -> soft to hard blend), tone (0-1 -> 400Hz-8200Hz LPF)]

cmos : [drive (0-1 -> 1x-8x gain), crunch (0-1 -> cubic non-linearity), fizz (0-1 -> 5th harmonic), pregain (0-1 -> 0.5x-2x), sag (0-1 -> envelope sag amount), tone (0-1 -> 400Hz-8200Hz LPF)]

Note: when adding new effects update Effects.h, Preset.cpp, Preset.h, PresetManager.cpp, PresetManager.h, and the Makefile
