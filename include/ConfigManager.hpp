// Handles loading json files for configs including weights and if a file is enabled
// Will create a method to add a file and set it's weight in a GUI
// If configs are not specified for a sample, create a new entry in SampleConfigs

#pragma once

#include <map>
#include <string>
#include <fstream>

#include "json.hpp"
#include "Configs.hpp"
#include "Error.hpp"

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();

    void readGlobalConfigs(GlobalConfigs &global); // Reads global configs from default file path
    void readSampleConfigs(std::map<std::string, SampleConfigs> &sample); // Reads all sample configs and adds new entries when needed

private:
    void addSampleConfig(); // Adds a new sample to the configs
    uint8_t rarityToWeight(std::string rarity);
    nlohmann::json data;

}; // end ConfigManager

/*

Global
{
    "global_volume": 1.0,
    "max_interval_s": 3600,
    "min_interval_s": 30,
    "sample_directory": "samples"
}



{
    "fnaf_jumpscare":
    {
        "fade_in_ms": 12.7
        "pan": 0.4
        "pitch": 1.0
        "rarity": legendary
        "volume": 0.3,
    }
}

*/