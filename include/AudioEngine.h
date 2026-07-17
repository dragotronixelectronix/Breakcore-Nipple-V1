#pragma once

#include <Arduino.h>
#include "DaisyDuino.h"
#include "Config.h"
#include <Beatrepeat.h>



enum effects {
    BEAT_REPEAT,
};

void ProcessAudio(float **in, float **processed, size_t size);