#pragma once

#include <cstdint>

#include <circle/bcm2835.h>
#include <circle/memio.h>


class FastGPIO
{
public:
        static inline void FastGPIODataWrite(u8 nValue, boolean setOutput) {
            // set pins 4-11 as output, leave rest as input
            u32 shift_value = static_cast<u32>(nValue) << 4;
            write32(ARM_GPIO_GPCLR0, 0xFF0u & ~shift_value);
            write32(ARM_GPIO_GPSET0, shift_value);
            if (setOutput) {
                write32(ARM_GPIO_GPFSEL0, 0x9249000u);
                write32(ARM_GPIO_GPFSEL1, 0x49u);
            }
        }

        static inline void FastGPIODataClear(void) {
            // be careful of GPIO 27 - it's holding OE on the shifters high!
            // reset data pins high 
            write32(ARM_GPIO_GPSET0, 0xFF0u);
            // set lowest 20 GPIO pins as input
            write32(ARM_GPIO_GPFSEL0, 0x0u);
            write32(ARM_GPIO_GPFSEL1, 0x0u);
            // clear values
            write32(ARM_GPIO_GPCLR0, 0xFF0u);
        }

        static inline void FastGPIOIRQSet(void) {
            // IRQ is GPIO 22
            // set GPIO 22 as output. also maintain GPIO 27 as output
            // clear it in case we still haven't cleared the last IRQ - we want it to hit
            write32(ARM_GPIO_GPCLR0, 0x400000u);
            //write32(ARM_GPIO_GPFSEL0 + 8, 0x200040u);
            // set 22 high
            write32(ARM_GPIO_GPSET0, 0x400000u);
        }

        static inline void FastGPIOIRQClear(void) {
            // set 22 back low
            write32(ARM_GPIO_GPCLR0, 0x400000u);
            // set 22 back to input, leaving 27 as output
            //write32(ARM_GPIO_GPFSEL0 + 8, 0x200000u);
        }

        static inline u32 FastGPIORead(void) {
            return read32(ARM_GPIO_GPLEV0);
        }
};
