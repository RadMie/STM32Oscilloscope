#include "stm32f10x.h"
#include <string.h>
#include "dso.h"
#include "tft.h"
#include "systick.h"
#include "key.h"
#include "fsmc_sram.h"

void UART_SendChar(char ch);
void UART_SendStr(char *str);
void USART_Configuration(void);
void LED_Configuration(void);

int main(void)
{
	SystemInit();
	USART_Configuration();
	LED_Configuration();
	FSMC_SRAM_Init();
	FSMC_SRAM_Test();

	delay_init();
	TFT_Init();
	TFT_SET_BACKLIGHT(255);
	TFT_CLEAR(Black);

	KEY_Configuration();
	DSO_Configuration();
	DSO_Initializtion();

	for(;;) {

		DSO_Run();

	}
}

void UART_SendChar(char ch) {
	while (!USART_GetFlagStatus(USART1,USART_FLAG_TC)); // wait for "Transmission Complete" flag cleared
	USART_SendData(USART1,ch);
}

void UART_SendStr(char *str) {
	while (*str) UART_SendChar(*str++);
}

void LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF , ENABLE);
/**
 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}


void USART_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}
