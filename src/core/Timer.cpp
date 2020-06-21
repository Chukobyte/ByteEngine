#include "Timer.h"

Timer::Timer(const std::string& timerId, Uint32 waitTimeInMilliseconds, bool loops) {
    this->id = timerId;
    this->waitTime = waitTimeInMilliseconds;
    this->loops = loops;
    startTicks = 0;
    pausedTicks = 0;
    isPaused = false;
    hasStarted = false;
}

std::string Timer::GetId() const {
    return id;
}

Uint32 Timer::GetWaitTime() const {
    return waitTime;
}

void Timer::SetWaitTime(Uint32 waitTimeInMilliseconds) {
    waitTime = waitTimeInMilliseconds;
}

bool Timer::DoesLoop() const {
    return loops;
}

void Timer::Start() {
    hasStarted = true;
    isPaused = false;
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Timer::Stop() {
    hasStarted = false;
    isPaused = false;
    startTicks = 0;
    pausedTicks = 0;
}

void Timer::Pause() {
    if(hasStarted && !isPaused) {
        isPaused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::UnPause() {
    if(hasStarted && isPaused) {
        isPaused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

bool Timer::HasStopped() const {
    return !hasStarted && !isPaused;
}

bool Timer::HasReachedTimeOut() const {
    Uint32 time = getTicks();
    if(time >= waitTime) {
        return true;
    }
    return false;
}

Uint32 Timer::getTicks() const {
    Uint32 time = 0;
    if(hasStarted) {
        if(isPaused) {
            time = pausedTicks;
        } else {
            time = SDL_GetTicks() - startTicks;
        }
    }
    return time;
}

bool Timer::HasStarted() const {
    return hasStarted;
}

bool Timer::IsPaused() const {
    return isPaused && hasStarted;
}
