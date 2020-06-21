#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <string>
#include <map>
#include <vector>

#include "../Timer.h"
#include "SignalManager.h"

class TimerManager {
  private:
    std::map<std::string, Timer*> timers;
    std::map<std::string, Timer*> activeTimers;
    SignalManager* signalManager;
    std::string timerSignalId = "timeout";

  public:
    TimerManager();
    TimerManager(SignalManager* signalManager);
    ~TimerManager();
    void CreateTimer(const std::string& timerId, Uint32 waitTimeInMilliSeconds, bool loops, bool startOnCreation);
    void StartTimer(const std::string& timerId);
    void StopTimer(const std::string& timerId);
    void PauseTimer(const std::string& timerId);
    void UnPauseTimer(const std::string& timerId);
    bool HasTimerStopped(const std::string& timerId);
    void SetWaitTime(const std::string& timerId, Uint32 waitTimeInMilliSeconds);
    void DeleteTimer(const std::string& timerId);
    void ClearAllTimers();
    std::vector<std::string> GetTimedOutTimers();
    void CheckTimerTimeouts();

};

#endif
