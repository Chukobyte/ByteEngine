#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include <string>

class Timer {
  private:
    std::string id;
    Uint32 waitTime;
    bool loops;
    Uint32 startTicks;
    Uint32 pausedTicks;
    bool isPaused;
    bool hasStarted;

  public:
    Timer(const std::string& timerId, Uint32 waitTimeInMilliseconds, bool loops);
    std::string GetId() const;
    Uint32 GetWaitTime() const;
    void SetWaitTime(Uint32 waitTimeInMilliseconds);
    bool DoesLoop() const;
    void Start();
    void Stop();
    void Pause();
    void UnPause();
    bool HasStopped() const;
    bool HasReachedTimeOut() const;
    Uint32 getTicks() const;
    bool HasStarted() const;
    bool IsPaused() const;
};

#endif
