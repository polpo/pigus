#pragma once

#include <cstdint>
#include <list>

#include <circle/timer.h>
#include <circle/logger.h>


typedef void Gus_EventHandler(uint32_t param1, void* param2);

class GusTimer
{
public:
    GusTimer(CLogger &logger);
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

    CLogger &m_Logger;

    std::list<TimerEvent> m_TimerEvents;
};
