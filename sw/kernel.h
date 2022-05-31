#pragma once

#include <stdint.h>

#include <circle/actled.h>
#include <circle/types.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/sched/scheduler.h>
#include <circle/logger.h>
#include <circle/spinlock.h>
#include <circle/gpiomanager.h>
#include <circle/cputhrottle.h>

#include "soundcardemu.h"

enum TShutdownMode
{
    ShutdownNone,
    ShutdownHalt,
    ShutdownReboot
};

class CKernel
{
public:
    CKernel (void);
    ~CKernel (void);

    boolean Initialize (void);
    TShutdownMode Run (void);

    CSpinLock *m_SpinLock;

private:
    CActLED m_ActLED;
    CKernelOptions m_Options;
    CDeviceNameService m_DeviceNameService;
    CTimer m_Timer;
    CLogger m_Logger;
    CScreenDevice m_Screen;
    CInterruptSystem m_Interrupt;
    CScheduler m_Scheduler;
    CGPIOManager m_Manager;
    CCPUThrottle m_CPUThrottle;

    SoundcardEmu* m_pSoundcardEmu;
};
