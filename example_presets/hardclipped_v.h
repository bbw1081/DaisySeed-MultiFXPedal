#pragma once

const char preset[] = R"({
    "name": "hardclipped v",
    "effects": ["hard_clip", "eq"],
    "params": [[0.9, 0.5, 0.5], [1, 0.5, 0, -0.5, 0, 0, -0.5, 0, 0.5, 1]],
    "description": "hard clipping distortion with a v-shaped eq"
})";
