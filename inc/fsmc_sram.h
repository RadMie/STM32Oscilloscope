/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               fsmc_sram.h
** Descriptions:            The FSMC SRAM application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-16
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define SRAM_BLANK1_START_ADDR  ((uint32_t)0x68000000)
#define SRAM_BLANK1_END_ADDR	((uint32_t)0x680FFFFF)

#define SRAM_BLANK2_START_ADDR  ((uint32_t)0x68100000)
#define SRAM_BLANK2_END_ADDR	((uint32_t)0x681FFFFF)

/* Private function prototypes -----------------------------------------------*/
void FSMC_SRAM_Init(void);
void FSMC_SRAM_Test(void);
void FSMC_SRAM_Test1(void);

#endif /* __FSMC_SRAM_H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
