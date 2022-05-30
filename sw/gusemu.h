#pragma once

#include <cstdint>

#include <circle/actled.h>
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/spinlock.h>

#include "soundcardemu.h"
#include "gus.h"


#define GUS_PORT 0x240u
#define GUS_PORT_BASE GUS_PORT - 0x200u

class GusEmu : public SoundcardEmu
{
public:
    GusEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CSpinLock &spinlock);
    ~GusEmu(void);

    boolean Initialize(void) override;

    TGPIOInterruptHandler* getIOWInterruptHandler() override;
    TGPIOInterruptHandler* getIORInterruptHandler() override;
    static void HandleIOWInterrupt(void *pParam);
    static void HandleIORInterrupt(void *pParam);

private:
    /* Gus *gus; */                
    std::unique_ptr<Gus> gus = nullptr;

    void RenderSound(s16* buffer, size_t nFrames) override;
};
