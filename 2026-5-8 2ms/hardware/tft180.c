#include "tft180.h"

void tft_init(void)
{
	tft180_clear(RGB565_YELLOW);
	tft180_set_dir(TFT180_PORTAIT);
	tft180_set_color(RGB565_WHITE,RGB565_BLACK);
	tft180_init();
}
