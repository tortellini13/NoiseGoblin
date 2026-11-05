#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include "miniaudio.h"

#include "Configs.hpp"
#include "Error.hpp"

inline void scanSampleFiles(SampleList &samples)
{
    namespace fs = std::filesystem;

    const fs::path audio_directory = "samples";

    // Create audio directory if it doesn't exist
    if (!fs::exists(audio_directory))
    {
        fs::create_directories(audio_directory);
        error("FileManager", "No audio files detected. Please add samples and run again");
    }

    // Supported file extensions
    const std::vector<std::string> supported_extensions =
    {
        ".wav",
        ".mp3",
        ".flac"
    };

    // Read directory
    if (std::filesystem::is_empty(audio_directory))
    {
        error("FileManager", "No audio files detected. Please add samples and run again");
        return;
    }

    for (const auto& entry : fs::directory_iterator(audio_directory))
    {
        if (!entry.is_regular_file())
            continue;

        const fs::path& path = entry.path();
        const std::string name = path.filename().string();
        const std::string extension = path.extension().string();

        // Check supported extension
        if (std::find(supported_extensions.begin(),
                      supported_extensions.end(),
                      extension) == supported_extensions.end())
        {
            error("FileManager", "Invalid sample file: " + path.filename().string() + ". Suppported file types are wav, mp3, and flac");
            continue;
        }

        // Verify file can be decoded by miniaudio
        ma_decoder decoder;

        ma_result result = ma_decoder_init_file(
            path.string().c_str(),
            nullptr,
            &decoder
        );

        if (result != MA_SUCCESS)
        {
            error("FileManager", "Invalid sample file: " + path.filename().string());
            continue;
        }

        ma_decoder_uninit(&decoder);

        // Add sample if it doesn't already exist
        samples.try_emplace(name, SampleConfigs{});
    }
} // end scanSampleFiles