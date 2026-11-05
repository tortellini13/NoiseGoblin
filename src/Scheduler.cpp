#include "Scheduler.hpp"

Scheduler::Scheduler(
    SampleList &samples,
    GlobalConfigs &configs,
    AudioManager &audio_manager)
    : samples(samples),
      configs(configs),
      audio_manager(audio_manager),
      rng(configs)
{
} // end Scheduler

Scheduler::~Scheduler()
{
    stop();
} // end ~Scheduler

//=====================================================================================

void Scheduler::start()
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (running)
            return;

        running = true;
    }

    scheduler_thread = std::thread(&Scheduler::run, this);
} // end start

void Scheduler::stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        running = false;
    }

    cv.notify_one();

    if (scheduler_thread.joinable())
        scheduler_thread.join();
} // end stop

//=====================================================================================

void Scheduler::run()
{
    while (true)
    {
        int interval_s = rng.chooseInterval(configs.max_interval_s, configs.min_interval_s);
        if (configs.debug_mode)
            std::cout << "Random sound playing in " << interval_s << " seconds\n";

        // Wait without occupying CPU
        {
            std::unique_lock<std::mutex> lock(mutex);

            if (cv.wait_for(
                    lock,
                    std::chrono::seconds(interval_s),
                    [this]
                    { return !running; }))
            {
                break;
            }
        }

        // Select sample
        std::string sample = rng.chooseSample(samples);

        if (!sample.empty())
        {
            sample_counter++;
            std::cout << "[" << sample_counter << "] ";
            audio_manager.playSample(sample, samples[sample]);
        }

        if (configs.debug_mode)
        {
            std::cout << "Fade in time [ms]: " << samples[sample].fade_in_ms.value() << "\n";

            std::cout << "Pan: ";
            float pan = samples[sample].pan.value();
            if (pan == 0)
                std::cout << "C" << pan;
            else
                (pan < 0) ? std::cout << "L" << pan : std::cout << "R" << pan;
            std::cout << "\n";
            std::cout << "Pitch [Octave Multiplier]: " << samples[sample].pitch.value() << "\n";
            std::cout << "Volume [0.0 - 1.0]: " << samples[sample].volume << "\n";
            uint8_t weight = samples[sample].weight;
            std::cout << "Weight: " << static_cast<int>(samples[sample].weight) << " | ";
            if (weight == Rarity::Common)
                std::cout << "common";
            else if (weight == Rarity::Uncommon)
                std::cout << "uncommon";
            else if (weight == Rarity::Rare)
                std::cout << "rare";
            else if (weight == Rarity::Epic)
                std::cout << "epic";
            else if (weight == Rarity::Legendary)
                std::cout << "legendary";
            else if (weight == Rarity::Mythic)
                std::cout << "mythic";
            else if (weight == Rarity::Divine)
                std::cout << "divine";
            else if (weight == Rarity::Disabled)
                std::cout << "disabled";
            std::cout << "\n\n";
        }
    }
} // end run

void Scheduler::wait()
{
    if (scheduler_thread.joinable())
        scheduler_thread.join();
} // end wait