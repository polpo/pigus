#pragma once

// #include "hardware/spi.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "stdio.h"

// SPI Defines
#define PIN_CS   9
#define PIN_SCK  10
#define PIN_MOSI 11
#define PIN_MISO 12

#include "pio_qspi.h"
#include "spi.pio.h"
/**
 * Super basic interface to SPI PSRAMs such as Espressif ESP-PSRAM64, apmemory APS6404L, IPUS IPS6404, Lyontek LY68L6400, etc.
 * NOTE that this abuses type punning to avoid shifts and masks to be as fast as possible!
 */
static constexpr uint32_t spi_to_qspi(uint8_t in) {
    /*
     *                         hgfedcba ->
     * 000b000a000d000c000f000e000h000g
     * 000h000g000f000e000d000c000b000a
     */
    return 
        ((in & 0b10000000) >>  3) |
        ((in & 0b01000000) >>  6)
        |
        ((in & 0b00100000) <<  7) |
        ((in & 0b00010000) <<  4)
        |
        ((in & 0b00001000) << 17) |
        ((in & 0b00000100) << 14)
        |
        ((in & 0b00000010) << 27) |
        ((in & 0b00000001) << 24)        
    ;
};
static pio_spi_inst_t spi;
class Psram {
    private:
        static constexpr uint8_t reset_en_cmd = 0x66u;
        static constexpr uint8_t reset_cmd = 0x99u;
        static constexpr uint8_t write_cmd = 0x02u;
        static constexpr uint8_t read_fast_cmd = 0x0bu;
    public:
        static void init(uint32_t baudrate) {
            // uint32_t reset_en_cmd32 = spi_to_qspi(reset_en_cmd);
            uint spi_offset = pio_add_program(pio0, &spi_fudge_program);
            uint spi_sm = pio_claim_unused_sm(pio0, true);
            spi.pio = pio0;
            spi.sm = spi_sm;

            gpio_set_drive_strength(PIN_CS, GPIO_DRIVE_STRENGTH_2MA);
            gpio_set_drive_strength(PIN_SCK, GPIO_DRIVE_STRENGTH_2MA);
            gpio_set_drive_strength(PIN_MOSI, GPIO_DRIVE_STRENGTH_2MA);
            gpio_set_slew_rate(PIN_CS, GPIO_SLEW_RATE_FAST);
            gpio_set_slew_rate(PIN_SCK, GPIO_SLEW_RATE_FAST);
            gpio_set_slew_rate(PIN_MOSI, GPIO_SLEW_RATE_FAST);

            pio_spi_fudge_cs_init(pio0, spi_sm, spi_offset, 8 /*n_bits*/, 1 /*clkdiv*/, PIN_SCK, PIN_MOSI, PIN_MISO);

            // SPI initialisation.
            printf("Inited SPI PIO... at sm %d\n", spi.sm);

            gpio_init(PIN_CS);
            gpio_set_dir(PIN_CS, GPIO_OUT);
            gpio_put(PIN_CS, 1);

            busy_wait_us(150);
            gpio_put(PIN_CS, 0);
            // pio_spi_write8_blocking(&spi, (unsigned char*)&reset_en_cmd32, 4);
            pio_spi_write_read_blocking(&spi, &reset_en_cmd, 1, nullptr, 0);
            gpio_put(PIN_CS, 1);
            busy_wait_us(50);
            gpio_put(PIN_CS, 0);
            // pio_spi_write8_blocking(&spi, (unsigned char*)&reset_cmd32, 4);
            pio_spi_write_read_blocking(&spi, &reset_cmd, 1, nullptr, 0);
            gpio_put(PIN_CS, 1);
            busy_wait_us(100);
        };

        static void set_baudrate(uint32_t baudrate) {
        }

        inline static void write8(uint32_t addr, uint8_t val) {
            unsigned char* addr_bytes = (unsigned char*)&addr;
            uint8_t command[5] = {
                write_cmd,
                *(addr_bytes + 2),
                *(addr_bytes + 1),
                *addr_bytes,
                val
            };

            // printf("%x %x %x %x\n", command[0], command[1], command[2], command[3]);

            // Select RAM chip
            gpio_put(PIN_CS, 0);
            // pio_spi_write8_blocking(&spi, (unsigned char*)&qwrite_cmd32, 4);
            // pio_spi_write8_blocking(&spi, command, sizeof(command));
            pio_spi_write_read_blocking(&spi, command, sizeof(command), nullptr, 0);
            // Deselect
            busy_wait_us(10);
            gpio_put(PIN_CS, 1);
        };

        inline static uint8_t read8(uint32_t addr) {
            uint8_t val; 
            unsigned char* addr_bytes = (unsigned char*)&addr;
            uint8_t command[5] = {
                read_fast_cmd,
                *(addr_bytes + 2),
                *(addr_bytes + 1),
                *addr_bytes,
                0,
            };

            // Select RAM chip
            gpio_put(PIN_CS, 0);
            // pio_spi_write8_blocking(&spi, (unsigned char*)&qread_cmd32, 4);
            // pio_spi_write8_blocking(&spi, command, sizeof(command));
            // pio_spi_read8_blocking(&spi, &val, 1);
            pio_spi_write_read_blocking(&spi, command, sizeof(command), &val, 1);
            /*
            uint8_t val0[32];
            pio_spi_read8_blocking(&spi, val0, 32);
            for(int i = 0; i < 32; ++i) {
                printf("%x ", val0[i]);
            }
            */
            // Deselect
            gpio_put(PIN_CS, 1);
            return val;
        };

        inline static void write16(uint32_t addr, uint16_t val) {
            unsigned char* addr_bytes = (unsigned char*)&addr;
            unsigned char* val_bytes = (unsigned char*)&val;
            uint8_t command[6] = {
                write_cmd,
                *(addr_bytes + 2),
                *(addr_bytes + 1),
                *addr_bytes,
                *val_bytes,
                *(val_bytes + 1)
            };

            // Select RAM chip
            gpio_put(PIN_CS, 0);
            pio_spi_write_read_blocking(&spi, command, sizeof(command), nullptr, 0);
            // Deselect
            busy_wait_us(10);
            gpio_put(PIN_CS, 1);
        };

        inline static uint16_t read16(uint32_t addr) {
            uint16_t val; 
            unsigned char* addr_bytes = (unsigned char*)&addr;
            uint8_t command[5] = {
                read_fast_cmd, // read
                *(addr_bytes + 2),
                *(addr_bytes + 1),
                *addr_bytes,
                0
            };

            // Select RAM chip
            gpio_put(PIN_CS, 0);
            // spi_write_blocking(SPI_PORT, command, sizeof(command));
            // spi_read_blocking(SPI_PORT, 0, (unsigned char*)&val, 2);
            pio_spi_write_read_blocking(&spi, command, sizeof(command), (unsigned char*)&val, 2);
            // Deselect
            gpio_put(PIN_CS, 1);
            return val;
        };

        inline static void write32(uint32_t addr, uint32_t val) {
            unsigned char* addr_bytes = (unsigned char*)&addr;
            unsigned char* val_bytes = (unsigned char*)&val;
            // Break the address into three bytes and send read command
            uint8_t command[8] = {
                write_cmd,
                *(addr_bytes + 2),
                *(addr_bytes + 1),
                *addr_bytes,
                *val_bytes,
                *(val_bytes + 1),
                *(val_bytes + 2),
                *(val_bytes + 3)
            };

            // Select RAM chip
            gpio_put(PIN_CS, 0);
            pio_spi_write_read_blocking(&spi, command, sizeof(command), nullptr, 0);
            // Deselect
            busy_wait_us(10);
            gpio_put(PIN_CS, 1);
        };

        inline static uint32_t read32(uint32_t addr) {
            uint32_t val;
            unsigned char* addr_bytes = (unsigned char*)&addr;
            uint8_t command[5] = {
                read_fast_cmd,
                *(addr_bytes + 2),
                *(addr_bytes + 1),
                *addr_bytes,
                0
            };

            // Select RAM chip
            gpio_put(PIN_CS, 0);
            // spi_write_blocking(SPI_PORT, command, 4);
            // spi_read_blocking(SPI_PORT, 0, (unsigned char*)&val, 4);
            pio_spi_write_read_blocking(&spi, command, sizeof(command), (unsigned char*)&val, 4);
            // Deselect
            gpio_put(PIN_CS, 1);
            return val;
        };
};

