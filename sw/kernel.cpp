#include "kernel.h"
//#include <circle/timer.h>
#include <cstddef>

#include <circle/gpiopin.h>
#include <circle/gpiopinfiq.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/synchronize.h>
#include <circle/multicore.h>

CKernel::CKernel(void)
:
    m_Timer(&m_Interrupt),
    m_Logger(4/*, &m_Timer*/),
    m_Screen(0, 0),
    m_AdlibEmu(CMemorySystem::Get(), &m_Interrupt, *m_SpinLock)
{
}

CKernel::~CKernel(void)
{
}

boolean CKernel::Initialize(void)
{
    m_Screen.Initialize();
    CDevice *pTarget = m_DeviceNameService.GetDevice (
            m_Options.GetLogDevice (), FALSE);
    if (pTarget == 0) {
        pTarget = &m_Screen;
    }
    m_Logger.Initialize(pTarget);
    m_Interrupt.Initialize();
    m_AdlibEmu.Initialize();
    return TRUE;
}

TShutdownMode CKernel::Run(void)
{
    m_Logger.Write("kernel", LogNotice, "init GPIO");
    CGPIOPinFIQ iow_pin(0, GPIOModeInput, &m_Interrupt);
    for (unsigned i=1; i < 32; ++i) {
        CGPIOPin pin(i, i == 27 ? GPIOModeOutput : GPIOModeInput);
        if (i == 27) {
            // Enable level shifters
            pin.Write(1);
        }
    }

    m_Logger.Write("kernel", LogNotice, "set up interrupts");
    iow_pin.ConnectInterrupt(AdlibEmu::HandleIOWInterrupt, &m_AdlibEmu);
    iow_pin.EnableInterrupt(GPIOInterruptOnFallingEdge);


    m_Logger.Write("kernel", LogNotice, "Running AdlibEmu");
    m_AdlibEmu.Run(0);

    return ShutdownReboot;
}

