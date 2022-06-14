#pragma once

#include <cstdint>
#include <array>

#include <circle/actled.h>
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/pwmsoundbasedevice.h>

#include "soundcardemu.h"
#include "gus.h"
#include "gustimer.h"
#include "vendor/speex/speex_resampler.h"


constexpr uint16_t GUS_PORT = 0x240;
constexpr uint16_t GUS_PORT_BASE = GUS_PORT - 0x200;

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
    static inline void RaiseIRQ(void* irq_param);

protected:
    void IOTask(void) override;
    void TimerTask(void) override;

private:
    /* Gus *gus; */                
    std::unique_ptr<Gus> gus = nullptr;
    std::array<CGPIOPin*, 8> m_DataPins;
    SpeexResamplerState* m_pResampler; 
    int m_ResamplerErr; 
    GusTimer m_GusTimer;

    void RenderSound(s16* buffer, size_t nFrames) override;
};
