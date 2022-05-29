#pragma once

#include <cstdint>

#include <circle/actled.h>
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/logger.h>
#include <circle/spinlock.h>
#include <circle/gpiopin.h>

#define SNDBUFSIZ 256

class SoundcardEmu : public CMultiCoreSupport
{
public:
	SoundcardEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock);
	~SoundcardEmu(void);

	virtual boolean Initialize(void);
	void Run(unsigned nCore);
	virtual TGPIOInterruptHandler* getIOWInterruptHandler() = 0;

protected:
	CLogger	&m_Logger;
	CInterruptSystem* m_pInterrupt;
	CSpinLock &m_SpinLock;

	virtual void RenderSound(s16* buffer, size_t nFrames) = 0;

private:
	CPWMSoundBaseDevice *m_sndDevice;

	void MainTask(void);
	void SoundTask(void); 
};
