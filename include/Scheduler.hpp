#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "RNG.hpp"
#include "AudioManager.hpp"
#include "Configs.hpp"

class Scheduler
{
public:
    Scheduler(
        SampleList& samples,
        GlobalConfigs& configs,
        AudioManager& audio_manager
    );

    ~Scheduler();

    void start();
    void stop();
    void wait();

private:
    void run();

    SampleList& samples;
    GlobalConfigs& configs;
    AudioManager& audio_manager;

    RNG rng;

    std::thread scheduler_thread;

    std::mutex mutex;
    std::condition_variable cv;

    bool running = false;
    int sample_counter = 0;
}; // end Scheduler