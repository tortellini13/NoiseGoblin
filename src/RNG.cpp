#include "RNG.hpp"

RNG::RNG(GlobalConfigs &configs) : engine(std::random_device{}()), gconfigs(configs)
{

} // end RNG

//=====================================================================================

std::string RNG::chooseSample(SampleList &samples)
{
    if (samples.empty())
        return "";

    std::vector<std::string> filenames;
    std::vector<double> weights;

    for (const auto &[filename, parameters] : samples)
    {
        // Skip recently played samples
        if (gconfigs.prevent_duplicates && std::find(prev_samples.begin(), prev_samples.end(), filename) != prev_samples.end())
            continue;

        filenames.push_back(filename);
        weights.push_back(parameters.weight);
    }

    // If all samples are excluded, allow selection from the full list
    if (filenames.empty())
    {
        for (const auto &[filename, parameters] : samples)
        {
            filenames.push_back(filename);
            weights.push_back(parameters.weight);
        }
    }

    std::discrete_distribution<> distribution(weights.begin(), weights.end());

    std::string selected = filenames[distribution(engine)];

    // Update recent sample history
    prev_samples.erase(prev_samples.begin());
    prev_samples.push_back(selected);

    return selected;
} // end chooseSamples

int RNG::chooseInterval(const int max_interval_s, const int min_interval_s)
{
    std::uniform_int_distribution<int> distribution;

    if (max_interval_s < min_interval_s)
    {
        error("RNG", "Max interval is greater than min, swapping values");
        distribution = std::uniform_int_distribution<int>(max_interval_s, min_interval_s);
    }

    else
        distribution = std::uniform_int_distribution<int>(min_interval_s, max_interval_s);

    return distribution(engine);
} // end chooseInterval