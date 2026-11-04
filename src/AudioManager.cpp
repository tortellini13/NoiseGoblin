#include "AudioManager.hpp"

AudioManager::AudioManager(GlobalConfigs& gconfigs) : gconfigs(gconfigs)
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

bool AudioManager::playSample(std::string audio_file, SampleConfigs configs)
{
    std::cout << "Playing file: " << audio_file << "\n";

    // Clean up previous sound, if one exists
    if (sound)
    {
        ma_sound_stop(sound.get());
        ma_sound_uninit(sound.get());
        sound.reset();

        if (gconfigs.debug_mode)
            std::cout << "Cleaning up previous ma_sound object\n";
    }

    // Allocate new sound
    sound = std::make_unique<ma_sound>();

    std::string file_path = "samples/" + audio_file;

    result = ma_sound_init_from_file(
        &engine,
        file_path.c_str(),
        0,
        NULL,
        NULL,
        sound.get()
    );

    if (result != MA_SUCCESS)
    {
        sound.reset();
        error("AudioManager", "Audio sample failed to load: " + audio_file);
        return false;
    }

    ma_sound_set_volume(sound.get(), configs.volume);

    if (configs.fade_in_ms.has_value())
        ma_sound_set_fade_in_milliseconds(
            sound.get(), 0, configs.volume, configs.fade_in_ms.value()
        );

    if (configs.pan.has_value())
        ma_sound_set_pan(sound.get(), configs.pan.value());

    if (configs.pitch.has_value())
        ma_sound_set_pitch(sound.get(), configs.pitch.value());

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