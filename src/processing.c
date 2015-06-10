#include "processing.h"

void PROCESSING_Configuration(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	/* USART1_TX -> PA9 , USART1_RX ->	PA10 */
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

void PROCESSING_SendChar(char ch) {
	while (!USART_GetFlagStatus(USART1,USART_FLAG_TC));
	USART_SendData(USART1,ch);
}

void PROCESSING_SendUint8(uint8_t uint) {
	while (!USART_GetFlagStatus(USART1,USART_FLAG_TC));
	USART_SendData(USART1,uint);
}

void PROCESSING_SendString(char *str) {
	while (*str) PROCESSING_SendChar(*str++);
}

void PROCESSING_SendInt(int32_t num) {
	char str[10]; // 10 chars max for INT32_MAX
	int i = 0;
	if (num < 0) {
		PROCESSING_SendChar('-');
		num *= -1;
	}
	if ((num < 10) && (num >= 0)) PROCESSING_SendChar('0');
	do str[i++] = num % 10 + '0'; while ((num /= 10) > 0);
	for (i--; i >= 0; i--) PROCESSING_SendChar(str[i]);
}

void PROCESSING_SendHex8(uint16_t num) {
	PROCESSING_SendChar(HEX_CHARS[(num >> 4)   % 0x10]);
	PROCESSING_SendChar(HEX_CHARS[(num & 0x0f) % 0x10]);
}

void PROCESSING_SendHex16(uint16_t num) {
	uint8_t i;
	for (i = 12; i > 0; i -= 4) PROCESSING_SendChar(HEX_CHARS[(num >> i) % 0x10]);
	PROCESSING_SendChar(HEX_CHARS[(num & 0x0f) % 0x10]);
}

void PROCESSING_SendHex32(uint32_t num) {
	uint8_t i;
	for (i = 28; i > 0; i -= 4)	PROCESSING_SendChar(HEX_CHARS[(num >> i) % 0x10]);
	PROCESSING_SendChar(HEX_CHARS[(num & 0x0f) % 0x10]);
}

void PROCESSING_SendBuffer(char *buf, uint16_t bufsize) {
	uint16_t i;
	for (i = 0; i < bufsize; i++) PROCESSING_SendChar(*buf++);
}

void PROCESSING_SendBufferPrintable(char *buf, uint16_t bufsize, char subst) {
	uint16_t i;
	char ch;
	for (i = 0; i < bufsize; i++) {
		ch = *buf++;
		PROCESSING_SendChar(ch > 32 ? ch : subst);
	}
}

void PROCESSING_SendBufferHex(char *buf, uint16_t bufsize) {
	uint16_t i;
	char ch;
	for (i = 0; i < bufsize; i++) {
		ch = *buf++;
		PROCESSING_SendChar(HEX_CHARS[(ch >> 4)   % 0x10]);
		PROCESSING_SendChar(HEX_CHARS[(ch & 0x0f) % 0x10]);
	}
}
