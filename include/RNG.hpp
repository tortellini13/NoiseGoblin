#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#include "Configs.hpp"
#include "Error.hpp"

class RNG
{
public:
    RNG();
    ~RNG() = default;

    std::string chooseSample(SampleList& samples);
    int chooseInterval(const int max_interval_s, const int min_interval_s);

private:
    std::mt19937 engine;
    std::vector<std::string> prev_samples{"", "", ""};
}; // end RNG