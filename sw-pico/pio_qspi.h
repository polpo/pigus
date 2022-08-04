/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PIO_SPI_H
#define _PIO_SPI_H

#include "hardware/pio.h"
#include "qspi.pio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pio_spi_inst {
    PIO pio;
    uint sm;
    uint cs_pin;
} pio_spi_inst_t;

void pio_spi_write_read_blocking(const pio_spi_inst_t *spi, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);
void pio_qspi_write_read_blocking(const pio_spi_inst_t *spi, uint32_t cmd, const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_len);

#ifdef __cplusplus
}
#endif

#endif
