#pragma once

#include <cstdint>

#include <circle/actled.h>
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/logger.h>
#include <circle/spinlock.h>

#define SNDBUFSIZ 1024

class AdlibEmu : public CMultiCoreSupport
{
public:
	AdlibEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock);
	~AdlibEmu(void);

	boolean Initialize(void);
	void Run(unsigned nCore);
	static void HandleIOWInterrupt(void *pParam);

private:
	CLogger	&m_Logger;
	CInterruptSystem* m_pInterrupt;
	CSpinLock &m_SpinLock;
	CPWMSoundBaseDevice *m_sndDevice;
	u8 adlibCommand[2];

	void MainTask(void);
	void SoundTask(void); 
};
