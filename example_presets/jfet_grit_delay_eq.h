#pragma once

const char preset[] = R"({
    "name": "jfet grit delay eq",
    "effects": ["jfet", "delay", "eq"],
    "params": [[0.7, 0.6, 0.5, 0.5], [400, 0.3, 0.4], [0.7, 0.6, 0.5, 0.5, 0.5, 0.5, 0.6, 0.7, 0.8, 0.9]],
    "description": "smooth jfet distortion with quick delay and treble-enhanced eq"
})";
