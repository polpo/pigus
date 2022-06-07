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

#define IPI_IOW IPI_USER
#define IPI_IOR IPI_USER + 1

#define SNDBUFSIZ 256

class SoundcardEmu : public CMultiCoreSupport
{
public:
	SoundcardEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock);
	~SoundcardEmu(void);

	virtual boolean Initialize(void);
	void Run(unsigned nCore);
	virtual TGPIOInterruptHandler* getIOWInterruptHandler() = 0;
	virtual TGPIOInterruptHandler* getIORInterruptHandler() = 0;

protected:
	CLogger	&m_Logger;
	CInterruptSystem* m_pInterrupt;
	CSpinLock &m_SpinLock;
#ifndef USE_INTERRUPTS
	u32 gpios;
#endif

	void IPIHandler(unsigned nCore, unsigned nIPI) override;
	virtual void RenderSound(s16* buffer, size_t nFrames) = 0;

private:
	CPWMSoundBaseDevice *m_sndDevice;

	void MainTask(void);
	void SoundTask(void); 
	void IOTask(void); 
};
