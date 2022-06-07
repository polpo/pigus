#include <circle/sysconfig.h>
#include <circle/memorymap.h>
#include <circle/gpiopin.h>

#include "opl.h"
#include "adlibemu.h"

AdlibEmu::AdlibEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock)
:
    SoundcardEmu(pMemorySystem, pInterrupt, spinlock)
{
}

AdlibEmu::~AdlibEmu(void) {}


boolean AdlibEmu::Initialize(void) 
{
    m_Logger.Write("AdlibEmu", LogNotice, "init adlib");
    adlib_init(44100);

    if (!SoundcardEmu::Initialize()) {
        return FALSE;
    }

    return TRUE;
}


void AdlibEmu::RenderSound(s16* buffer, size_t nFrames)
{
    adlib_getsample(buffer, nFrames);
}


TGPIOInterruptHandler* AdlibEmu::getIOWInterruptHandler()
{
    return AdlibEmu::HandleIOWInterrupt;
}


TGPIOInterruptHandler* AdlibEmu::getIORInterruptHandler()
{
    return AdlibEmu::HandleIORInterrupt;
}


void AdlibEmu::HandleIOWInterrupt(void *pParam)
{
    AdlibEmu* pThis = static_cast<AdlibEmu*>(pParam);
#ifdef USE_INTERRUPTS
    // If we are using interrupts, read GPIOs here. Otherwise will already be populated.
    u32 gpios = CGPIOPin::ReadAll();
#else
    u32 gpios = pThis->gpios;
#endif
    switch ((gpios >> 12) & 0x3FF) {
        case 0x388:
            /* CActLED::Get()->On(); */
	    pThis->adlibCommand[0] = (gpios >> 4) & 0xFF;
            break;
        case 0x389:
            /* CActLED::Get()->Off(); */
	    pThis->adlibCommand[1] = (gpios >> 4) & 0xFF;
            adlib_write(pThis->adlibCommand[0], pThis->adlibCommand[1]);
	    DataSyncBarrier();
            break;
    }
}

