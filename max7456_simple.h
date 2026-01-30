#pragma once
#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;

    GPIO_TypeDef *rst_port;   // можно NULL если reset нет
    uint16_t rst_pin;

    uint8_t is_pal;           // 1=PAL, 0=NTSC
} max7456_t;

void max7456_init(max7456_t *d);
void max7456_clear(max7456_t *d);

// x: 0..29, y: 0..15 (условно)
void max7456_putc(max7456_t *d, uint8_t x, uint8_t y, uint8_t ch);
void max7456_puts(max7456_t *d, uint8_t x, uint8_t y, const char *s);
