// Loads a specified sample and plays it with miniaudio

#include "miniaudio.h"
#include <string>
#include <algorithm>
#include <chrono>

#include "Configs.hpp"
#include "Error.hpp"

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    void setGlobalVolume(float globalVolume);
    bool playSample(std::string audio_file, SampleConfigs configs);

private:
    static void onPlaybackFinished(void *userData, ma_sound *sound);
    ma_result result;
    ma_engine engine;
    std::unique_ptr<ma_sound> sound;
}; // end AudioManager