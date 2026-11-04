#pragma once

#include <optional>
#include <string>
#include <map>
#include <cstdint>

enum Rarity : uint8_t
{
    Common = 100,
    Uncommon = 75,
    Rare = 50,
    Epic = 25,
    Legendary = 10,
    Mythic = 5,
    Divine = 1,
    Disabled = 0
}; // end Rarity

struct GlobalConfigs
{
    bool debug_mode = false;
    float global_volume = 1.0;
    int max_interval_s = 60 * 60 * 2; // 2 hours
    int min_interval_s = 60 * 10;     // 10 mins
}; // end GlobalConfigs

struct SampleConfigs
{
    std::optional<float> fade_in_ms = 0.0; // [0ms, any]
    std::optional<float> pan = 0.0;        // [-1.0, 1.0]
    std::optional<float> pitch = 1.0;      // [0.5, 2.0] [1 oct lower, 1 oct higher]
    float volume = 1.0f;                   // [0.0, 1.0]
    uint8_t weight = 100;
}; // end SampleConfigs

using SampleList = std::map<std::string, SampleConfigs>;