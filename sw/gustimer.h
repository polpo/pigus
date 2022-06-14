#pragma once

#include <cstdint>
#include <list>

#include <circle/timer.h>


typedef void Gus_EventHandler(uint32_t param1, void* param2);

class GusTimer
{
public:
    GusTimer();
    ~GusTimer(void);

    void AddEvent(Gus_EventHandler* eventHandler, double delay, uint32_t param1 = 0, void* param2 = 0);
    void RemoveEvents(Gus_EventHandler* eventHandler);

    void TimerTask(void);

private:
    struct TimerEvent {
        unsigned start;
        unsigned delay;
        Gus_EventHandler* handler;
        uint32_t param1;
        void* param2;
    };

    std::list<TimerEvent> m_TimerEvents;
};
