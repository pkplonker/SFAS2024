#pragma once
#include <chrono>
#include <iostream>
#include <string>

#include "Implementation/Logging/Debug.h"

class Timer {
public:
    Timer(const std::string& name) : name(name), start(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::string message = name + ": " + NanoSecondsToStringMs(duration) + " ms";
        Trace(message)
    }
    std::string NanoSecondsToStringMs(long long nanoseconds) {
        double milliseconds = nanoseconds / 1000000.0;

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(4) << milliseconds;

        return stream.str();
    }

private:
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
