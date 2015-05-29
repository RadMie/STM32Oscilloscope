#include "stm32f10x.h"
#include "dso.h"
#include "Key.h"
#include "tft.h"
#include "graphic.h"
#include "systick.h"

int main(void)
{
	delay_init();
	TFT_Init();
	TFT_SET_BACKLIGHT(255);
	TFT_CLEAR(Black);

	Key_Initializtion();
	DSO_Configuration();
	DSO_Initializtion();

	for(;;) {

		DSO_Run();
	}
}
