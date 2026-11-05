#include <map>
#include <string>

#include "Configs.hpp"   
#include "FileManager.hpp"
#include "ConfigManager.hpp"
#include "AudioManager.hpp"
#include "Scheduler.hpp"
#include "Error.hpp"

int main()
{
    SampleList samples;
    scanSampleFiles(samples);

    GlobalConfigs global_configs;
    ConfigManager config_manager;

    config_manager.readGlobalConfigs(global_configs);
    if (samples.size() < 4)
    {
        error("NoiseGoblin", "Less than 4 samples found, removing duplicate prevention");
        global_configs.prevent_duplicates = false;
    }
    config_manager.readSampleConfigs(samples);

    AudioManager audio_manager(global_configs);
    Scheduler scheduler(samples, global_configs, audio_manager);

    scheduler.start();
    scheduler.wait();

    return 0;
} // end main