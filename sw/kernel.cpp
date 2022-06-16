#include "kernel.h"
#include <circle/timer.h>
#include <cstddef>

#include <circle/gpiopin.h>
#include <circle/gpiopinfiq.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/synchronize.h>
#include <circle/multicore.h>

//#define EMULATE_ADLIB

#ifdef EMULATE_ADLIB
#include "adlibemu.h"
#else
#include "gusemu.h"
#endif

CKernel::CKernel(void)
:
    /* m_CPUThrottle(CPUSpeedLow), */
    m_Timer(&m_Interrupt),
    m_Logger(4/*, &m_Timer*/),
#ifdef USE_BUFFERED_SCREEN
    m_ScreenUnbuffered(0, 0),
    m_Screen(&m_ScreenUnbuffered),
#else
    m_Screen(0, 0),
#endif
    m_Manager(&m_Interrupt)
{
#ifdef EMULATE_ADLIB
    m_pSoundcardEmu = new AdlibEmu(CMemorySystem::Get(), &m_Interrupt, m_Timer);
#else
    m_pSoundcardEmu = new GusEmu(CMemorySystem::Get(), &m_Interrupt, m_Timer);
#endif
}

CKernel::~CKernel(void)
{
}

boolean CKernel::Initialize(void)
{
#ifdef USE_BUFFERED_SCREEN
    m_ScreenUnbuffered.Initialize();
#else
    m_Screen.Initialize();
#endif
    CDevice *pTarget = &m_Screen;
    /*
    CDevice *pTarget = m_DeviceNameService.GetDevice (
            m_Options.GetLogDevice (), FALSE);
    if (pTarget == 0) {
        pTarget = &m_Screen;
    }
    */
    m_Logger.Initialize(pTarget);
    m_Logger.Write("kernel", LogNotice, "Compile time: " __DATE__ " " __TIME__);
#ifdef USE_BUFFERED_SCREEN
    m_Logger.RegisterPanicHandler(PanicHandler);
#endif
    m_Interrupt.Initialize();
    m_Manager.Initialize();
    m_pSoundcardEmu->Initialize();
    m_CPUThrottle.SetSpeed(CPUSpeedMaximum);
    return TRUE;
}

TShutdownMode CKernel::Run(void)
{
    m_Logger.Write("kernel", LogNotice, "init GPIO");
#if defined USE_INTERRUPTS  || defined USE_HYBRID_POLLING
    CGPIOPin iow_pin(0, GPIOModeInput, &m_Manager);
    CGPIOPinFIQ ior_pin(1, GPIOModeInput, &m_Interrupt);
    /* CGPIOPinFIQ iow_pin(0, GPIOModeInput, &m_Interrupt); */
    /* CGPIOPin ior_pin(1, GPIOModeInput, &m_Manager); */
#else
    CGPIOPin iow_pin(0, GPIOModeInput);
    CGPIOPin ior_pin(1, GPIOModeInput);
#endif
    for (unsigned i=2; i < 32; ++i) {
        CGPIOPin pin(i, (i == 27 || i == 22) ? GPIOModeOutput : GPIOModeInput);
        if (i == 27) {
            // Enable level shifters
            pin.Write(1);
        }
        if (i == 22) {
            pin.Write(0);
        }
    }


#if defined USE_INTERRUPTS || defined USE_HYBRID_POLLING
    m_Logger.Write("kernel", LogNotice, "set up interrupts");
#ifdef USE_INTERRUPTS
    TGPIOInterruptHandler* iowHandler = m_pSoundcardEmu->getIOWInterruptHandler();
    if (iowHandler) {
	iow_pin.ConnectInterrupt(iowHandler, m_pSoundcardEmu);
	iow_pin.EnableInterrupt(GPIOInterruptOnFallingEdge);
    }
#endif
    TGPIOInterruptHandler* iorHandler = m_pSoundcardEmu->getIORInterruptHandler();
    if (iorHandler) {
	ior_pin.ConnectInterrupt(iorHandler, m_pSoundcardEmu);
	ior_pin.EnableInterrupt(GPIOInterruptOnFallingEdge);
	ior_pin.EnableInterrupt2(GPIOInterruptOnRisingEdge);
    }
#endif

    m_Logger.Write("kernel", LogNotice, "Running SoundcardEmu");
#ifdef USE_BUFFERED_SCREEN
    CScheduler* const pScheduler = CScheduler::Get();
    m_Logger.Write("kernel", LogNotice, "Using buffered screen");
    for (;;) {
	m_Screen.Update ();
        pScheduler->Yield();
    }
#else
    m_pSoundcardEmu->Run(0);
#endif

    return ShutdownReboot;
}


#ifdef USE_BUFFERED_SCREEN

void CKernel::PanicHandler (void)		// called on a system panic condition
{
    /*
	EnableIRQs ();				// go to TASK_LEVEL

	s_pThis->m_Screen.Update (2000);	// display all messages before system halt
    */
}

#endif
