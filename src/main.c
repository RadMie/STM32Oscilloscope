#include "stm32f10x.h"
#include <string.h>
#include "dso.h"
#include "tft.h"
#include "systick.h"
#include "key.h"
#include "fsmc_sram.h"
#include "processing.h"

void LED_Configuration(void);

int main(void)
{
	SystemInit();
	PROCESSING_Configuration();
	LED_Configuration();
	FSMC_SRAM_Init();
	delay_init();
	TFT_Init();
	KEY_Configuration();
	DSO_Configuration();
	DSO_Initializtion();

	for(;;) {

		DSO_Run();
	}
}

void LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF , ENABLE);
  /* LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}
