#pragma once

#include <cstdint>

#include <circle/bcm2835.h>
#include <circle/memio.h>


class FastGPIO
{
public:
        static inline void FastGPIOWriteData(u8 nValue, boolean setOutput) {
            // set pins 4-11 as output, leave rest as input
            u32 shift_value = static_cast<u32>(nValue) << 4;
            write32(ARM_GPIO_GPCLR0, 0xFF0u & ~shift_value);
            write32(ARM_GPIO_GPSET0, shift_value);
            if (setOutput) {
                write32(ARM_GPIO_GPFSEL0, 0x9249000u);
                write32(ARM_GPIO_GPFSEL1, 0x49u);
            }
        }

        static inline void FastGPIOClear(void) {
            // be careful of GPIO 27 - it's holding OE on the shifters high!
            // reset data pins high 
            write32(ARM_GPIO_GPSET0, 0xFF0u);
            // set lowest 20 GPIO pins as input
            write32(ARM_GPIO_GPFSEL0, 0x0u);
            write32(ARM_GPIO_GPFSEL1, 0x0u);
            // clear values
            write32(ARM_GPIO_GPCLR0, 0xFF0u);
        }

        static inline u32 FastGPIORead(void) {
            return read32(ARM_GPIO_GPLEV0);
        }
};
