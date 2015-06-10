#ifndef __PROCESSING_H
#define __PROCESSING_H

#include "stm32f10x.h"

#define HEX_CHARS      "0123456789ABCDEF"

void PROCESSING_Configuration(void);
void PROCESSING_SendChar(char ch);
void PROCESSING_SendUint8(uint8_t uint);
void PROCESSING_SendString(char *str);
void PROCESSING_SendInt(int32_t num);
void PROCESSING_SendHex8(uint16_t num);
void PROCESSING_SendHex16(uint16_t num);
void PROCESSING_SendHex32(uint32_t num);
void PROCESSING_SendBuffer(char *buf, uint16_t bufsize);
void PROCESSING_SendBufferPrintable(char *buf, uint16_t bufsize, char subst);
void PROCESSING_SendBufferHex(char *buf, uint16_t bufsize);

#endif
