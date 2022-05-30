#pragma once

#include <cstdint>

#include <circle/actled.h>
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/logger.h>
#include <circle/spinlock.h>

#include "soundcardemu.h"


class AdlibEmu : public SoundcardEmu
{
public:
	AdlibEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock);
	~AdlibEmu(void);
	
	boolean Initialize(void) override;

	TGPIOInterruptHandler* getIOWInterruptHandler() override;
	TGPIOInterruptHandler* getIORInterruptHandler() override;
	static void HandleIOWInterrupt(void *pParam);

private:
	u8 adlibCommand[2];

	void RenderSound(s16* buffer, size_t nFrames) override;
};
