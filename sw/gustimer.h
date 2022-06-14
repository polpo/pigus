#pragma once

#include <cstdint>
#include <list>

#include <circle/timer.h>


#define GUS_PORT 0x240
#define GUS_PORT_BASE 0x40

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
        TimerEvent(unsigned _start, unsigned _delay, Gus_EventHandler* _handler, uint32_t _param1, void* _param2) :
            start(_start), delay(_delay), param1(_param1), param2(_param2) {}
        unsigned start;
        unsigned delay;
        Gus_EventHandler* handler;
        uint32_t param1;
        void* param2;
    };

    std::list<TimerEvent> m_TimerEvents;
};
