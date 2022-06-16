#include "gustimer.h"


GusTimer::GusTimer(CLogger& logger)
:
    m_Logger(logger)
{}

GusTimer::~GusTimer(void) {}


void GusTimer::AddEvent(Gus_EventHandler* eventHandler, double delay, uint32_t param1, void* param2) {
    unsigned delayTicks = delay * 1000 * (1000000U / CLOCKHZ);
    m_TimerEvents.push_back({CTimer::GetClockTicks(), delayTicks, eventHandler, param1, param2}); 
    /* m_Logger.Write("GusTimer", LogNotice, "Timer with delay %f added", delay); */
}

void GusTimer::RemoveEvents(Gus_EventHandler* eventHandler) {
    m_TimerEvents.remove_if([eventHandler](TimerEvent& event) { return event.handler == eventHandler; });
}

void GusTimer::TimerTask(void) {
    unsigned nTicks;

    for (;;) {
        nTicks = CTimer::GetClockTicks();

        auto event = m_TimerEvents.begin();
        auto end = m_TimerEvents.end();

        while (event != end) {
            if (nTicks - event->start >= event->delay) {
                (*event->handler)(event->param1, event->param2);
                event = m_TimerEvents.erase(event);
                /* m_Logger.Write("GusTimer", LogNotice, "Timer fired"); */
            } else {
                ++event;
            }
        }
    }
}
