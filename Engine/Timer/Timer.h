#pragma once
#include <chrono>
using namespace std;
using namespace chrono;

class Timer {

  public:
    Timer();
    double GetMillisecondsElapsed();
    void Restart();
    bool Stop();
    bool Start();

  private:
    bool isRunning = false;
    time_point<steady_clock> start;
    time_point<steady_clock> stop;
};