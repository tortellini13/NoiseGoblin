#include "ConfigManager.hpp"

ConfigManager::ConfigManager()
{

} // end ConfigManager

ConfigManager::~ConfigManager()
{

} // end ~ConfigManger

//=====================================================================================

void ConfigManager::readGlobalConfigs(GlobalConfigs &global)
{
    /*
    - Check if configs folder exists. If not, create new folder
    - Check if GlobalConfigs.json exists. If not, create file
    - Read GlobalConfigs.json and store data in global struct
    - If parameter does not have value, write an entry from DefaultConfig
    */

    // Check if config folder exists and create it if not
    if (!std::filesystem::exists("config"))
    {
        error("ConfigManager", "Configs folder does not exist, creating one");
        std::filesystem::create_directories("config");
    }

    // Check for GlobalConfig.json and create it if it doesn't exist
    if (!std::filesystem::exists("config/GlobalConfig.json"))
    {
        error("ConfigManager", "GlobalConfig.json does not exist, creating file");

        GlobalConfigs defaults;

        data["global_volume"] = defaults.global_volume;
        data["max_interval_s"] = defaults.max_interval_s;
        data["min_interval_s"] = defaults.min_interval_s;
        data["sample_directory"] = defaults.sample_directory;

        std::ofstream globalConfigFile("config/GlobalConfig.json");
        if (!globalConfigFile)
        {
            error("ConfigManager", "Unable to create GlobalConfig.json");
            return;
        }

        globalConfigFile << data.dump(4);
        globalConfigFile.close();
    }

    // Open GlobalConfig.json
    std::ifstream globalConfigFile("config/GlobalConfig.json");
    if (!globalConfigFile)
    {
        error("ConfigManager", "Unable to open GlobalConfig.json");
        return;
    }

    try
    {
        data = nlohmann::json::parse(globalConfigFile);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        error("ConfigManager", "Invalid GlobalConfig.json: " + std::string(e.what()));
        return;
    }

    // Populate missing settings with defaults
    GlobalConfigs defaults;

    data["global_volume"] = data.value("global_volume", defaults.global_volume);
    data["max_interval_s"] = data.value("max_interval_s", defaults.max_interval_s);
    data["min_interval_s"] = data.value("min_interval_s", defaults.min_interval_s);
    data["sample_directory"] = data.value("sample_directory", defaults.sample_directory);

    // Store global config values
    global.global_volume = data["global_volume"];
    global.max_interval_s = data["max_interval_s"];
    global.min_interval_s = data["min_interval_s"];
    global.sample_directory = data["sample_directory"];

    // Write updated config back to file
    std::ofstream updatedConfigFile("config/GlobalConfig.json");
    if (updatedConfigFile)
    {
        updatedConfigFile << data.dump(4);
    }
} // end readGlobalConfigs

void ConfigManager::readSampleConfigs(std::map<std::string, SampleConfigs> &sample)
{
    /*
    - Check if SampleConfigs.json exists. If not, create file
    - Read all configs into a map <"file name", SampleParams>
    - Compare map to the list of files found
    - Add a new entry into SampleConfigs.json if file found but no entry
    - Remove from map if entry found but no file
    */
} // end readSampleConfigs

void ConfigManager::addSampleConfig()
{

} // addSampleConfig

uint8_t ConfigManager::rarityToWeight(std::string rarity)
{
    uint8_t weight;
    if (rarity == "common")
        return static_cast<uint8_t>(Rarity::Common);
    else if (rarity == "uncommon")
        return static_cast<uint8_t>(Rarity::Uncommon);
    else if (rarity == "rare")
        return static_cast<uint8_t>(Rarity::Rare);
    else if (rarity == "epic")
        return static_cast<uint8_t>(Rarity::Epic);
    else if (rarity == "legendary")
        return static_cast<uint8_t>(Rarity::Legendary);
    else if (rarity == "mythic")
        return static_cast<uint8_t>(Rarity::Mythic);
    else if (rarity == "divine")
        return static_cast<uint8_t>(Rarity::Divine);

    if (weight != 0)
        return weight;

    error("ConfigManager", "Invalid sample rarity: " + rarity + "... Using common rarity");
    return static_cast<uint8_t>(Rarity::Common);
} // end raritytoWeight