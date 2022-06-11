#pragma once

#include <cstdint>
#include <array>

#include <circle/actled.h>
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/pwmsoundbasedevice.h>
#include <circle/spinlock.h>

#include "soundcardemu.h"
#include "gus.h"
#include "vendor/speex/speex_resampler.h"


#define GUS_PORT 0x240
#define GUS_PORT_BASE 0x40

class GusEmu : public SoundcardEmu
{
public:
    GusEmu(CMemorySystem* pMemorySystem, CInterruptSystem* pInterrupt, CTimer &timer);
    ~GusEmu(void);

    boolean Initialize(void) override;

    TGPIOInterruptHandler* getIOWInterruptHandler() override;
    TGPIOInterruptHandler* getIORInterruptHandler() override;
    static inline void HandleIOWInterrupt(void *pParam);
    static inline void HandleIORInterrupt(void *pParam);

protected:
    void IOTask(void) override;

private:
    /* Gus *gus; */                
    std::unique_ptr<Gus> gus = nullptr;
    std::array<CGPIOPin*, 8> m_DataPins;
    SpeexResamplerState* m_pResampler; 
    int m_ResamplerErr; 

    void RenderSound(s16* buffer, size_t nFrames) override;
};
