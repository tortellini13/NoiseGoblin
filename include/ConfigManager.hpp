#pragma once

#include <map>
#include <string>
#include <fstream>
#include <cmath>

#include "json.hpp"
#include "Configs.hpp"
#include "Error.hpp"

class ConfigManager
{
public:
    ConfigManager() = default;
    ~ConfigManager() = default;

    void readGlobalConfigs(GlobalConfigs &global); // Reads global configs from default file path
    void readSampleConfigs(SampleList &samples); // Reads all sample configs and adds new entries when needed

private:
    // void addSampleConfig(std::string sampleName, SampleConfigs config); // Adds a new sample to the configs
    uint8_t rarityToWeight(std::string rarity);
    std::string weightToRarity(uint8_t weight);
}; // end ConfigManager