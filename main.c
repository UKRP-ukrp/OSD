#include "max7456_simple.h"
#include "osd_app.h"

max7456_t osd;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI1_Init();

  osd.hspi = &hspi1;
  osd.cs_port = GPIOA;
  osd.cs_pin  = GPIO_PIN_4;  // CS = PA4
  osd.rst_port = GPIOB;
  osd.rst_pin  = GPIO_PIN_0; // RST = PB0
  osd.is_pal = 0;            // 0=NTSC, 1=PAL

  max7456_init(&osd);

  // Рисуем фиксированную строку
  osd_draw_fixed_dbm(&osd);

  while (1)
  {
    HAL_Delay(1000);
  }
}
