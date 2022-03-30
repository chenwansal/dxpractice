#include "Timer.h"

Timer::Timer() {
    start = high_resolution_clock::now();
    stop = high_resolution_clock::now();
}

double Timer::GetMillisecondsElapsed() {
    if (isRunning) {
        auto elapsed =
            duration<double, milli>(high_resolution_clock::now() - start);
        return elapsed.count();
    } else {
        auto elapsed = duration<double, milli>(stop - start);
        return elapsed.count();
    }
}

float Timer::GetDeltaTime() {
    float delta = (float)this->GetMillisecondsElapsed();
    return delta * 0.001f;
}

void Timer::Restart() {
    this->isRunning = true;
    start = high_resolution_clock::now();
}

bool Timer::Stop() {
    if (!isRunning) {
        return false;
    } else {
        stop = high_resolution_clock::now();
        this->isRunning = false;
        return true;
    }
}

bool Timer::Start() {
    if (isRunning) {
        return false;
    } else {
        start = high_resolution_clock::now();
        this->isRunning = true;
        return true;
    }
}
