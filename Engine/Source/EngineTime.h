#pragma once
#include <chrono>


class EngineTime
{
public:
    static long long GetElapsed()
    {
        Init();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - startTime;
        return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    }

    static std::string Format(long long time)
    {
        Init();
        int hours = time / 3600;
        int minutes = (time % 3600) / 60;
        int seconds = time % 60;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << hours << ":"
            << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << seconds;

        return oss.str();
    }

    static std::string GetElapsedFormatted()
    {
        Init();
        return Format(GetElapsed());
    }

    static void Init()
    {
        if (!init)
        {
            startTime = std::chrono::steady_clock::now();
            init = true;
        }
    }

    static void Update()
    {
        frames++;
    }


    static std::string GetCurrentTime()
    {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::tm now_tm{};
        localtime_s(&now_tm, &currentTime);

        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%H:%M:%S");

        return oss.str();
    }

    inline static unsigned long long CurrentFrame() { return frames; }

private:
    static inline std::chrono::steady_clock::time_point startTime;
    static inline bool init = false;
    static inline unsigned long long frames = 0;
};
