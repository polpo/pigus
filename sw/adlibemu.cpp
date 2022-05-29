#include <circle/sysconfig.h>
#include <circle/memorymap.h>
#include <circle/sched/scheduler.h>
#include <circle/gpiopin.h>
#include <circle/gpiopinfiq.h>

#include "opl.h"
#include "adlibemu.h"

AdlibEmu::AdlibEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock)
:
    CMultiCoreSupport(pMemorySystem),
    m_Logger(*CLogger::Get()),
    m_pInterrupt(pInterrupt),
    m_SpinLock(spinlock),
    m_sndDevice(nullptr)
{
}

AdlibEmu::~AdlibEmu(void) {}

boolean AdlibEmu::Initialize(void) 
{
    m_Logger.Write("AdlibEmu", LogNotice, "init adlib");
    adlib_init(44100);

    m_Logger.Write("AdlibEmu", LogNotice, "init sound");
    m_sndDevice = new CPWMSoundBaseDevice(m_pInterrupt, 44100, 256);
    m_sndDevice->SetWriteFormat(SoundFormatSigned16, 2);
    if (!m_sndDevice->AllocateQueueFrames(256)) {
        m_Logger.Write("AdlibEmu", LogPanic, "Failed to allocate sound queue");
    }
    m_sndDevice->Start();

    if (!CMultiCoreSupport::Initialize()) {
        return FALSE;
    }

    return TRUE;
}

void AdlibEmu::MainTask(void) {
    m_Logger.Write("AdlibEmu", LogNotice, "MainTask starting up (and not doing anything - using interrupts!)");
    CScheduler* const pScheduler = CScheduler::Get();
    for (;;) {
        pScheduler->Yield();
    }
}

void AdlibEmu::SoundTask(void) {
    m_Logger.Write("AdlibEmu", LogNotice, "SoundTask starting up");

    const u8 nChannels = 2;
    const u8 nBytesPerSample = sizeof(int16_t);
    const u8 nBytesPerFrame = nChannels * nBytesPerSample;

    const size_t nQueueSizeFrames = m_sndDevice->GetQueueSizeFrames();

    s16 AdlibBuffer[nQueueSizeFrames * nChannels];

    for (;;) {
        const size_t nFrames = nQueueSizeFrames - m_sndDevice->GetQueueFramesAvail();
        const size_t nWriteBytes = nFrames * nBytesPerFrame;

        adlib_getsample(AdlibBuffer, nFrames);

        const int nResult = m_sndDevice->Write(AdlibBuffer, nWriteBytes);
        if (nResult != static_cast<int>(nWriteBytes)) {
            m_Logger.Write("AdlibEmu", LogError, "Sound data dropped");
        }
    }
}

void AdlibEmu::Run(unsigned nCore) {
    switch (nCore) {
    case 0:
        return MainTask();
    case 1:
        return SoundTask();
    default:
        return;
    }
}


void AdlibEmu::HandleIOWInterrupt(void *pParam)
{
    AdlibEmu* pThis = static_cast<AdlibEmu*>(pParam);
    u32 porta = CGPIOPin::ReadAll();
    switch ((porta >> 12) & 0x3FF) {
        case 0x388:
            /* CActLED::Get()->On(); */
	    pThis->adlibCommand[0] = (porta >> 4) & 0xFF;
            break;
        case 0x389:
            /* CActLED::Get()->Off(); */
	    pThis->adlibCommand[1] = (porta >> 4) & 0xFF;
            adlib_write(pThis->adlibCommand[0], pThis->adlibCommand[1]);
	    DataSyncBarrier();
            break;
    }
}

