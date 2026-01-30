#include "osd_app.h"
#include <string.h>

void osd_draw_fixed_dbm(max7456_t *osd)
{
    const char *txt = "0.00 dBm";

    // Правый верхний угол: y=0, x = 30 - len
    const uint8_t y = 0;
    uint8_t len = (uint8_t)strlen(txt);
    uint8_t x = (len <= 30) ? (uint8_t)(30 - len) : 0;

    max7456_puts(osd, x, y, txt);
}
