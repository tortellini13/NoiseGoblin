#pragma once

#include "miniaudio.h"
#include <string>
#include <algorithm>
#include <chrono>

#include "Configs.hpp"
#include "Error.hpp"

class AudioManager
{
public:
    AudioManager(GlobalConfigs &configs);
    ~AudioManager();

    bool playSample(std::string audio_file, SampleConfigs configs);

private:
    GlobalConfigs gconfigs;
    ma_result result;
    ma_engine engine;
    std::unique_ptr<ma_sound> sound;
}; // end AudioManager