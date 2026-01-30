#include "max7456_simple.h"
#include <string.h>

// ===== MAX7456 registers =====
#define REG_VM0   0x00
#define REG_VM1   0x01
#define REG_DMM   0x04
#define REG_DMAH  0x05
#define REG_DMAL  0x06
#define REG_DMDI  0x07

// VM0 bits
#define VM0_RESET      0x02
#define VM0_OSD_ENABLE 0x08
#define VM0_PAL        0x40
#define VM0_NTSC       0x00

// DMM bits
#define DMM_AUTOINC    0x01
#define DMM_CLEAR      0x04

static inline void cs_low(max7456_t *d)  { HAL_GPIO_WritePin(d->cs_port, d->cs_pin, GPIO_PIN_RESET); }
static inline void cs_high(max7456_t *d) { HAL_GPIO_WritePin(d->cs_port, d->cs_pin, GPIO_PIN_SET); }

static void wr(max7456_t *d, uint8_t reg, uint8_t val)
{
    uint8_t tx[2] = { reg, val };
    cs_low(d);
    HAL_SPI_Transmit(d->hspi, tx, 2, 10);
    cs_high(d);
}

static void hw_reset(max7456_t *d)
{
    if (!d->rst_port) return;
    HAL_GPIO_WritePin(d->rst_port, d->rst_pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(d->rst_port, d->rst_pin, GPIO_PIN_SET);
    HAL_Delay(20);
}

static void sw_reset(max7456_t *d)
{
    wr(d, REG_VM0, VM0_RESET);
    HAL_Delay(5);
}

void max7456_clear(max7456_t *d)
{
    wr(d, REG_DMM, DMM_CLEAR);
    HAL_Delay(20);
}

void max7456_init(max7456_t *d)
{
    // CS idle high
    cs_high(d);

    hw_reset(d);
    sw_reset(d);

    // включаем OSD и задаем видеорежим
    uint8_t vm0 = VM0_OSD_ENABLE | (d->is_pal ? VM0_PAL : VM0_NTSC);
    wr(d, REG_VM0, vm0);

    // (опционально) VM1 можно не трогать на старте
    // wr(d, REG_VM1, 0x00);

    max7456_clear(d);
}

void max7456_putc(max7456_t *d, uint8_t x, uint8_t y, uint8_t ch)
{
    // Стандартная “текстовая” сетка: 30 колонок.
    // addr = y*30 + x
    const uint16_t addr = (uint16_t)y * 30u + x;

    wr(d, REG_DMAH, (addr >> 8) & 0x01);
    wr(d, REG_DMAL, (addr >> 0) & 0xFF);

    wr(d, REG_DMM, DMM_AUTOINC);
    wr(d, REG_DMDI, ch);
}

void max7456_puts(max7456_t *d, uint8_t x, uint8_t y, const char *s)
{
    while (*s && x < 30) {
        // На многих платах MAX7456 цифры/буквы совпадают с ASCII.
        // Если у тебя “кракозябры” — надо будет маппинг/шрифт.
        max7456_putc(d, x++, y, (uint8_t)(*s++));
    }
}
