#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

#include "stm32f10x.h"

#define SRAM_BLANK1_START_ADDR  ((uint32_t)0x68000000)
#define SRAM_BLANK1_END_ADDR	((uint32_t)0x680FFFFF)

#define SRAM_BLANK2_START_ADDR  ((uint32_t)0x68100000)
#define SRAM_BLANK2_END_ADDR	((uint32_t)0x681FFFFF)

void FSMC_SRAM_Init(void);
void FSMC_SRAM_Test(void);

#endif /* __FSMC_SRAM_H */
