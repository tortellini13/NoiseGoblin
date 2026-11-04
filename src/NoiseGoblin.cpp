#include <map>
#include <string>

#include "Configs.hpp"   
#include "FileManager.hpp"
#include "ConfigManager.hpp"
#include "AudioManager.hpp"
#include "Scheduler.hpp"

int main()
{
    SampleList samples;

    if (!scanSampleFiles(samples))
        return -1;

    GlobalConfigs global_configs;
    ConfigManager config_manager;

    config_manager.readGlobalConfigs(global_configs);
    config_manager.readSampleConfigs(samples);

    AudioManager audio_manager(global_configs);

    Scheduler scheduler(samples, global_configs, audio_manager);

    scheduler.start();
    scheduler.wait();

    // scheduler.stop();

    return 0;
} // end main