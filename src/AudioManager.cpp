#pragma once

#include "AudioManager.hpp"

AudioManager::AudioManager()
{
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
        error("AudioManager", "Failed to initialize ma_engine", true);

} // end AudioManager

AudioManager::~AudioManager()
{
    ma_engine_uninit(&engine);
} // end ~AudioManager

//=====================================================================================

void AudioManager::setGlobalVolume(float globalVolume)
{
    float volume = std::clamp(globalVolume, 0.0f, 1.0f);
    ma_engine_set_volume(&engine, volume);
} // end setGlobalVolume

bool AudioManager::playSample(std::string audio_file, SampleConfigs configs)
{
    // Load sound from file
    sound = std::make_unique<ma_sound>();
    result = ma_sound_init_from_file(&engine, audio_file.c_str(), 0, NULL, NULL, sound.get());

    if (result != MA_SUCCESS)
    {
        sound.reset();
        error("AudioManager", "Audio sample favoid setSampleParams(SampleParams params)iled to load");
        return false;
    }

    // Set callback for when sound finishes playing
    ma_sound_set_end_callback(sound.get(), onPlaybackFinished, this);

    // Set sample parameters and play sound
    ma_sound_set_volume(sound.get(), configs.volume);
    (configs.fade_in_ms.has_value()) ? ma_sound_set_fade_in_milliseconds(sound.get(), 0, configs.volume, configs.fade_in_ms.value()) : error("AudioManager", "Sample fade in time not specified");
    (configs.pan.has_value()) ? ma_sound_set_pan(sound.get(), configs.pan.value()) : error("AudioManager", "Sample pan not specified");
    (configs.pitch.has_value()) ? ma_sound_set_pitch(sound.get(), configs.pitch.value()) : error("AudioManager", "Sample pitch not specified");

    result = ma_sound_start(sound.get());

    if (result != MA_SUCCESS)
    {
        ma_sound_uninit(sound.get());
        sound.reset();
        error("AudioManager", "Audio sample failed to start");
        return false;
    }

    return true;
} // end playSample

void AudioManager::onPlaybackFinished(void *userData, ma_sound *sound)
{
    ma_sound_uninit(sound);
} // end onPlaybackFinished