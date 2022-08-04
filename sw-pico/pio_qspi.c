/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pio_qspi.h"
#include <stdio.h>

// Just 8 bit functions provided here. The PIO program supports any frame size
// 1...32, but the software to do the necessary FIFO shuffling is left as an
// exercise for the reader :)
//
// Likewise we only provide MSB-first here. To do LSB-first, you need to
// - Do shifts when reading from the FIFO, for general case n != 8, 16, 32
// - Do a narrow read at a one halfword or 3 byte offset for n == 16, 8
// in order to get the read data correctly justified. 

void __time_critical_func(pio_spi_write_read_blocking)(const pio_spi_inst_t *spi,
                                                       const uint8_t *src, const size_t src_len,
                                                       uint8_t *dst, const size_t dst_len) {
    size_t tx_remain = src_len, rx_remain = dst_len;

    /* printf("put X "); */
    pio_sm_put_blocking(spi->pio, spi->sm, src_len * 8);
    /* printf("put Y "); */
    pio_sm_put_blocking(spi->pio, spi->sm, dst_len * 8);

    /* printf("writing: "); */
    io_rw_8 *txfifo = (io_rw_8 *) &spi->pio->txf[spi->sm];
    while (tx_remain) {
        if (!pio_sm_is_tx_fifo_full(spi->pio, spi->sm)) {
            /* printf("%x ", *src); */
            *txfifo = *src++;
            --tx_remain;
        }
    }

    /* printf("reading: "); */
    io_rw_8 *rxfifo = (io_rw_8 *) &spi->pio->rxf[spi->sm];
    while (rx_remain) {
        if (!pio_sm_is_rx_fifo_empty(spi->pio, spi->sm)) {
            *dst++ = *rxfifo;
            /* printf("%x", *(dst - 1)); */
            --rx_remain;
        }
    }
    /* printf("done\n"); */
}

void __time_critical_func(pio_qspi_write_read_blocking)(const pio_spi_inst_t *spi, uint32_t cmd, const uint8_t *src, size_t src_len, uint8_t *dst,
                                                         size_t dst_len) {

    size_t tx_remain = src_len, rx_remain = dst_len;

    printf("put X ");
    pio_sm_put(spi->pio, spi->sm, src_len);
    printf("put Y ");
    pio_sm_put(spi->pio, spi->sm, dst_len);
    printf("put cmd ");
    pio_sm_put(spi->pio, spi->sm, cmd);

    printf("writing: ");
    io_rw_8 *txfifo = (io_rw_8 *) &spi->pio->txf[spi->sm];
    while (tx_remain) {
        if (!pio_sm_is_tx_fifo_full(spi->pio, spi->sm)) {
            *txfifo = *src++;
            --tx_remain;
            printf(".");
        }
    }

    printf("reading: ");
    io_rw_8 *rxfifo = (io_rw_8 *) &spi->pio->rxf[spi->sm];
    while (rx_remain) {
        if (!pio_sm_is_rx_fifo_empty(spi->pio, spi->sm)) {
            *dst++ = *rxfifo;
            --rx_remain;
            printf(".");
        }
    }
    printf("done\n");
}

