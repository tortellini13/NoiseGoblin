#include "ConfigManager.hpp"

void ConfigManager::readGlobalConfigs(GlobalConfigs &global)
{
    // Check if config folder exists and create it if not
    if (!std::filesystem::exists("configs"))
    {
        error("ConfigManager", "Configs folder does not exist, creating one");
        std::filesystem::create_directories("configs");
    }

    // Check for GlobalConfig.json and create it if it doesn't exist
    nlohmann::json global_data;
    if (!std::filesystem::exists("configs/GlobalConfig.json"))
    {
        error("ConfigManager", "GlobalConfig.json does not exist, creating file");

        GlobalConfigs defaults;

        global_data["debug_mode"] = defaults.debug_mode;
        global_data["global_volume"] = defaults.global_volume;
        global_data["max_interval_s"] = defaults.max_interval_s;
        global_data["min_interval_s"] = defaults.min_interval_s;
        global_data["prevent_duplicates"] = defaults.prevent_duplicates;

        std::ofstream global_config_file("configs/GlobalConfig.json");
        if (!global_config_file)
        {
            error("ConfigManager", "Unable to create GlobalConfig.json");
            return;
        }

        global_config_file << global_data.dump(4);
        global_config_file.close();

        return;
    }

    // Open GlobalConfig.json
    std::ifstream global_config_file("configs/GlobalConfig.json");
    if (!global_config_file)
    {
        error("ConfigManager", "Unable to open GlobalConfig.json");
        return;
    }

    try
    {
        global_data = nlohmann::json::parse(global_config_file);
    }

    catch (const nlohmann::json::parse_error &e)
    {
        error("ConfigManager", "Invalid GlobalConfig.json: " + std::string(e.what()));
        return;
    }

    // Populate missing settings with defaults
    GlobalConfigs defaults;

    global_data["debug_mode"] = global_data.value("debug_mode", defaults.debug_mode);
    global_data["global_volume"] = global_data.value("global_volume", defaults.global_volume);
    global_data["max_interval_s"] = global_data.value("max_interval_s", defaults.max_interval_s);
    global_data["min_interval_s"] = global_data.value("min_interval_s", defaults.min_interval_s);
    global_data["prevent_duplicates"] = global_data.value("prevent_duplicates", defaults.prevent_duplicates);

    // Store global config values
    global.debug_mode = global_data["debug_mode"];
    global.global_volume = global_data["global_volume"];
    global.max_interval_s = global_data["max_interval_s"];
    global.min_interval_s = global_data["min_interval_s"];
    global.prevent_duplicates = global_data["prevent_duplicates"];

    // Write updated config back to file
    std::ofstream updated_configs("configs/GlobalConfig.json");
    if (updated_configs)
        updated_configs << global_data.dump(4);
} // end readGlobalConfigs

void ConfigManager::readSampleConfigs(SampleList &samples)
{
    // Check for SampleConfig.json and create it if it doesn't exist
    if (!std::filesystem::exists("configs/SampleConfig.json"))
    {
        error("ConfigManager", "SampleConfig.json does not exist, creating file");

        std::ofstream sample_config_file("configs/SampleConfig.json");
        if (!sample_config_file)
        {
            error("ConfigManager", "Unable to create GlobalConfig.json");
            return;
        }

        return;
    }

    // Load SampleConfigs.json
    nlohmann::json sample_data = nlohmann::json::object();
    std::ifstream sample_config_file("configs/SampleConfig.json");
    if (sample_config_file.peek() != std::ifstream::traits_type::eof())
        sample_data = nlohmann::json::parse(sample_config_file);

    // Load data from file into memory
    SampleList file_config;
    for (const auto &[filename, parameters] : sample_data.items())
    {
        SampleConfigs config;

        // Loop through each sample listed
        for (const auto &[key, value] : parameters.items())
        {
            if (key == "fade_in_ms")
                config.fade_in_ms = value.get<float>();

            else if (key == "pan")
                config.pan = value.get<float>();

            else if (key == "pitch")
                config.pitch = value.get<float>();

            else if (key == "volume")
                config.volume = value.get<float>();

            else if (key == "rarity")
            {
                if (value.is_string())
                    config.weight = rarityToWeight(value.get<std::string>());
                else
                {
                    error("ConfigManager", "Invalid rarity for " + filename + ": expected a string, received " + value.dump());
                    config.weight = Rarity::Common;
                }
            }

            else
                error("ConfigManager", "Invalid config: " + value.dump());
        }

        file_config[filename] = config; // Store config
    }

    // Compare configs read to audio files detected and store configs when file is found. If config is not found, create a new entry
    nlohmann::json updated_configs_json;
    for (auto &[key, parameters] : samples)
    {
        auto it = file_config.find(key);

        SampleConfigs default_vals;
        parameters = (it != file_config.end()) ? it->second : default_vals;

        // Write current configs to json onbject
        updated_configs_json[key] = {
            {"fade_in_ms", parameters.fade_in_ms},
            {"pan", parameters.pan},
            {"pitch", parameters.pitch},
            {"rarity", weightToRarity(parameters.weight)},
            {"volume", parameters.volume}};
    }

    // Write sample configs back to file to update with newly found audio files
    std::ofstream updated_configs("configs/SampleConfig.json");
    if (updated_configs)
        updated_configs << std::fixed << std::setprecision(2) << updated_configs_json.dump(4);
} // end readSampleConfigs

//=====================================================================================

uint8_t ConfigManager::rarityToWeight(std::string rarity)
{
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
    else if (rarity == "disabled")
        return static_cast<uint8_t>(Rarity::Disabled);

    error("ConfigManager", "Invalid rarity: " + rarity + "... Using common rarity");
    return static_cast<uint8_t>(Rarity::Common);
} // end raritytoWeight

std::string ConfigManager::weightToRarity(uint8_t weight)
{
    if (weight == Rarity::Common)
        return "common";
    else if (weight == Rarity::Uncommon)
        return "uncommon";
    else if (weight == Rarity::Rare)
        return "rare";
    else if (weight == Rarity::Epic)
        return "epic";
    else if (weight == Rarity::Legendary)
        return "legendary";
    else if (weight == Rarity::Mythic)
        return "mythic";
    else if (weight == Rarity::Divine)
        return "divine";
    else if (weight == Rarity::Disabled)
        return "disabled";

    error("ConfigManager", "Invalid weight: " + std::to_string(weight) + "... Using common rarity");
    return "common";
} // end weightToRarity