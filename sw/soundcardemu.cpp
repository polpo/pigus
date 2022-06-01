#include <circle/sysconfig.h>
#include <circle/memorymap.h>
#include <circle/sched/scheduler.h>
#include <circle/gpiopin.h>
#include <circle/gpiopinfiq.h>

#include "soundcardemu.h"

SoundcardEmu::SoundcardEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock)
:
    CMultiCoreSupport(pMemorySystem),
    m_Logger(*CLogger::Get()),
    m_pInterrupt(pInterrupt),
    m_SpinLock(spinlock),
    m_sndDevice(nullptr)
{
}


SoundcardEmu::~SoundcardEmu(void) {}


boolean SoundcardEmu::Initialize(void) 
{
    m_Logger.Write("SoundcardEmu", LogNotice, "init sound");
    m_sndDevice = new CPWMSoundBaseDevice(m_pInterrupt, 44100, SNDBUFSIZ);
    m_sndDevice->SetWriteFormat(SoundFormatSigned16, 2);
    if (!m_sndDevice->AllocateQueueFrames(SNDBUFSIZ)) {
        m_Logger.Write("SoundcardEmu", LogPanic, "Failed to allocate sound queue");
    }
    m_sndDevice->Start();

    if (!CMultiCoreSupport::Initialize()) {
        return FALSE;
    }

    return TRUE;
}


void SoundcardEmu::MainTask(void) {
#ifdef USE_INTERRUPTS
    m_Logger.Write("SoundcardEmu", LogNotice, "MainTask starting up (and not doing anything - using interrupts!)");
#else
    m_Logger.Write("SoundcardEmu", LogNotice, "MainTask starting up (and not doing anything - using polling in IOTask!)");
#endif
    CScheduler* const pScheduler = CScheduler::Get();
    for (;;) {
        pScheduler->Yield();
    }
}


void SoundcardEmu::SoundTask(void) {
    m_Logger.Write("SoundcardEmu", LogNotice, "SoundTask starting up");

    const u8 nChannels = 2;
    const u8 nBytesPerSample = sizeof(s16);
    const u8 nBytesPerFrame = nChannels * nBytesPerSample;

    const size_t nQueueSizeFrames = m_sndDevice->GetQueueSizeFrames();

    s16 SoundBuffer[nQueueSizeFrames * nChannels];

    for (;;) {
        const size_t nFrames = nQueueSizeFrames - m_sndDevice->GetQueueFramesAvail();
        const size_t nWriteBytes = nFrames * nBytesPerFrame;

	RenderSound(SoundBuffer, nFrames);

        const int nResult = m_sndDevice->Write(SoundBuffer, nWriteBytes);
        if (nResult != static_cast<int>(nWriteBytes)) {
            m_Logger.Write("SoundcardEmu", LogError, "Sound data dropped");
        }
    }
}


void SoundcardEmu::IOTask(void) {
    m_Logger.Write("SoundcardEmu", LogNotice, "IOTask starting up");

    u8 curr_iow, last_iow = 1, curr_ior, last_ior = 1;
    u32 gpios;

    for (;;) {
	gpios = CGPIOPin::ReadAll();
	curr_iow = gpios & 0x1;
	curr_ior = (gpios & 0x2) >> 0x1;

	if (curr_iow < last_iow) {  // falling edge of ~IOW
	    getIOWInterruptHandler()(this);
	}

	if (curr_ior < last_ior) {  // falling edge of ~IOR
	}

	last_iow = curr_iow;
	last_ior = curr_ior;
    }
}


void SoundcardEmu::Run(unsigned nCore) {
    switch (nCore) {
    case 0:
        return MainTask();
    case 1:
        return SoundTask();
#ifndef USE_INTERRUPTS
    case 2:
        return IOTask();
#endif
    default:
        return;
    }
}
