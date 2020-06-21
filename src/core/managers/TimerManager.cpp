#include "TimerManager.h"

TimerManager::TimerManager() {}

TimerManager::TimerManager(SignalManager* signalManager) : signalManager(signalManager) {}

TimerManager::~TimerManager() {}

void TimerManager::CreateTimer(const std::string& timerId, Uint32 waitTimeInMilliSeconds, bool loops, bool startOnCreation) {
    Timer* timer = new Timer(timerId, waitTimeInMilliSeconds, loops);
    timers.emplace(timerId, timer);
    signalManager->CreateSignal(timerId, timerSignalId);
    if(startOnCreation) {
        StartTimer(timerId);
    }
}

void TimerManager::StartTimer(const std::string& timerId) {
    if(timers.count(timerId) > 0) {
        timers[timerId]->Start();
        activeTimers.emplace(timerId, timers[timerId]);
    }
}

void TimerManager::PauseTimer(const std::string& timerId) {
    if(timers.count(timerId) > 0) {
        timers[timerId]->Pause();
    }
}

void TimerManager::UnPauseTimer(const std::string& timerId) {
    if(timers.count(timerId) > 0) {
        timers[timerId]->UnPause();
    }
}

void TimerManager::StopTimer(const std::string& timerId) {
    if(timers.count(timerId) > 0) {
        timers[timerId]->Stop();
        activeTimers.erase(timerId);
    }
}

bool TimerManager::HasTimerStopped(const std::string& timerId) {
    if(timers.count(timerId) > 0) {
        return timers[timerId]->HasStopped();
    }
    return false;
}

void TimerManager::SetWaitTime(const std::string& timerId, Uint32 waitTimeInMilliSeconds) {
    if(timers.count(timerId) > 0) {
        timers[timerId]->SetWaitTime(waitTimeInMilliSeconds);
    }
}

void TimerManager::DeleteTimer(const std::string& timerId) {
    if(timers.count(timerId) > 0) {
        timers[timerId]->Stop();
        signalManager->RemoveEntitySignal(timerId);
        delete timers[timerId];
        timers.erase(timerId);
        activeTimers.erase(timerId);
    }
}

void TimerManager::ClearAllTimers() {
    for (auto const& timerRef : activeTimers) {
        Timer* activeTimer = timerRef.second;
        DeleteTimer(activeTimer->GetId());
    }
}

void TimerManager::CheckTimerTimeouts() {
    for (auto const& timerRef : activeTimers) {
        Timer* activeTimer = timerRef.second;
        if(activeTimer->HasReachedTimeOut()) {
            activeTimer->Stop();
            signalManager->EmitSignal(activeTimer->GetId(), timerSignalId);
            if(activeTimer->DoesLoop()) {
                activeTimer->Start();
            } else {
                activeTimers.erase(activeTimer->GetId());
            }
        }
    }
}
