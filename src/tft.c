#include "stm32f10x.h"
#include "tft.h"
#include "ascii.h"
#include <stdio.h>
#include <string.h>

#define LCD_REG              (*((volatile unsigned short *) 0x6C000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x6C000002)) /* RS = 1 */

/************************************************************************
 * 			FUNCTION:			TFT_FSMC_CONFIG
 ************************************************************************/
static void TFT_FSMC_CONFIG(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 5;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 5;
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;	  
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
	/* Enable FSMC Bank4_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}


/************************************************************************
 * 			FUNCTION:			LCD_WriteCommand
 ************************************************************************/
void LCD_WriteCommand(uint16_t cmd)
{
	/* Write cmd */
	LCD_REG = cmd;
}

/************************************************************************
 * 			FUNCTION:			LCD_WriteCommand
 ************************************************************************/
void LCD_WriteData(uint16_t data)
{
	/* Write 16-bit data */
	LCD_RAM = data;
}

/************************************************************************
 * 			FUNCTION:			LCD_WriteCommand
 ************************************************************************/
static __inline uint16_t LCD_ReadData(void)
{  
	/* Read 16-bit data */
	return LCD_RAM;
}

/*******************************************************************************
* Function Name  : SSD1963_SSD1963_GPIO_WR
* Description    : Set a GPIO pin to state high(1) or low(0)
* Input          : - pin: LCD_RESET or LCD_SPENA or LCD_SPCLK  or LCD_SPDAT
*                  - state: 0 for low and 1 for high
* Output         : None
* Return         : None
* Attention		 : Set the GPIO pin an output prior using this function
*******************************************************************************/
static void SSD1963_GPIO_WR(uint8_t pin, uint8_t state)
{
	static uint8_t _gpioStatus = 0;  /* ssd1963 specific */
	
	if( state )
	{
		_gpioStatus = _gpioStatus | pin;
	}
	else
	{
		_gpioStatus = _gpioStatus & ( ~pin );
	}
	
	LCD_WriteCommand(0xBA);		     /* Set GPIO value */
	LCD_WriteData(_gpioStatus);
}

/************************************************************************
 * 			FUNCTION:			SSD1963_SPI_Write
 ************************************************************************/
static void SSD1963_SPI_Write(uint8_t byte)
{
	uint8_t bit_ctr;
	
	for( bit_ctr = 0; bit_ctr < 8; bit_ctr++ )
	{
		if( byte & 0x80 )
		{
			SSD1963_GPIO_WR(LCD_SPDAT,1);
		}
		else
		{
			SSD1963_GPIO_WR(LCD_SPDAT,0);
		}
		SSD1963_GPIO_WR(LCD_SPCLK,0);
		SSD1963_GPIO_WR(LCD_SPCLK,1);
		byte = ( byte << 1 );
	}
}

/************************************************************************
 * 			FUNCTION:			SSD1963_SPI_WriteReg
 ************************************************************************/

static void SSD1963_SPI_WriteReg(uint8_t reg, uint16_t cmd)
{
	SSD1963_GPIO_WR(LCD_SPENA,0);
	cmd = ( (reg << 10) | (1 << 9) | cmd );
	SSD1963_SPI_Write( ( (uint8_t) (cmd)>>8) );
	SSD1963_SPI_Write( ( (uint8_t) cmd) );
	SSD1963_GPIO_WR(LCD_SPENA,1);
}

/************************************************************************
 * 			FUNCTION:			delay_ms
 ************************************************************************/

static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 

/************************************************************************
 * 			FUNCTION:			TFT_INIT
 ************************************************************************/

void TFT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
	                       RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

	/* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
	 PD.10(D15), PD.14(D0), PD.15(D1) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Set PE.05(A21), PE.06(A22) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Set PF.00(A0 (RS)) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* Set PG.12(NE4) as alternate function push pull - CE3(LCD /CS) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	TFT_FSMC_CONFIG();

	/* Set MN(multipliers) of PLL, VCO = crystal freq * (N+1) */
	/* PLL freq = VCO/M with 250MHz < VCO < 800MHz */
	/* The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq */
	LCD_WriteCommand(0xE2);			    /* Set PLL with OSC = 10MHz (hardware) */
	/* Multiplier N = 35, VCO (>250MHz)= OSC*(N+1), VCO = 360MHz */
	LCD_WriteData(0x23);
	LCD_WriteData(0x02);				/* Divider M = 2, PLL = 360/(M+1) = 120MHz */
	LCD_WriteData(0x54);				/* Validate M and N values */	
	
	LCD_WriteCommand(0xE0);			    /* Start PLL command */
	LCD_WriteData(0x01);				/* enable PLL */	
	
	delay_ms(10);						/* wait stablize */
	LCD_WriteCommand(0xE0);			    /* Start PLL command again */
	LCD_WriteData(0x03);				/* now, use PLL output as system clock */		
	/* once PLL locked (at 120MHz), the data hold time set shortest */
	LCD_WriteCommand(0x01);			    /* Soft reset */
	delay_ms(10);
	
	/* Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously */
	/* Typical DCLK for TYX350TFT320240 is 6.5MHz in 24 bit format */
	/* 6.5MHz = 120MHz*(LCDC_FPR+1)/2^20 */
	/* LCDC_FPR = 56796 (0x00DDDC) */
	LCD_WriteCommand(0xE6);
	LCD_WriteData(0x00);
	LCD_WriteData(0xDD);
	LCD_WriteData(0xDC);
	
	/* Set panel mode, varies from individual manufacturer */
	LCD_WriteCommand(0xB0);
	
	LCD_WriteData(0x20);				/* set 24-bit 3.5" TFT Panel */
	LCD_WriteData(0x00);				/* set Hsync+Vsync mode */
	LCD_WriteData((DISP_HOR_RESOLUTION-1)>>8&0x07); /* Set panel size */
	LCD_WriteData((DISP_HOR_RESOLUTION-1)&0xff);
	LCD_WriteData((DISP_VER_RESOLUTION-1)>>8&0x07);
	LCD_WriteData((DISP_VER_RESOLUTION-1)&0xff);
	LCD_WriteData(0x00);				/* RGB sequence */
	
	/* Set horizontal period */
	LCD_WriteCommand(0xB4);
	
	#define HT ( DISP_HOR_RESOLUTION + DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH + DISP_HOR_FRONT_PORCH )
	LCD_WriteData((HT-1)>>8);	
	LCD_WriteData(HT-1);
	
	#define HPS ( DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH )
	LCD_WriteData((HPS-1)>>8);
	LCD_WriteData(HPS-1);
	LCD_WriteData(DISP_HOR_PULSE_WIDTH-1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	
	/* Set vertical period */
	LCD_WriteCommand(0xB6);
	
	#define VT ( DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION )
	LCD_WriteData((VT-1)>>8);
	LCD_WriteData(VT-1);
	
	#define VSP ( DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH )
	LCD_WriteData((VSP-1)>>8);
	LCD_WriteData(VSP-1);
	LCD_WriteData(DISP_VER_PULSE_WIDTH-1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	
	/* Set pixel data interface */
	LCD_WriteCommand(0xF0);
	
	#ifdef USE_16BIT_PMP
	LCD_WriteData(0x03);		/* 16-bit(565 format) data */
	#else
	LCD_WriteData(0x00);		/* 8-bit data for 16bpp */
	#endif
	
	LCD_WriteCommand(0xB8);	    /* Set all GPIOs to output, controlled by host */
	LCD_WriteData(0x0f);		/* Set GPIO0 as output */
	LCD_WriteData(0x01);		/* GPIO[3:0] used as normal GPIOs */
	
	/* LL Reset to LCD */
	SSD1963_GPIO_WR(LCD_SPENA, 1);
	SSD1963_GPIO_WR(LCD_SPCLK, 1);
	SSD1963_GPIO_WR(LCD_SPDAT,1);
	SSD1963_GPIO_WR(LCD_RESET,1);
	SSD1963_GPIO_WR(LCD_RESET,0);
	delay_ms(1);
	SSD1963_GPIO_WR(LCD_RESET,1);
	
	SSD1963_SPI_WriteReg(0x00,0x07);
	SSD1963_SPI_WriteReg(0x01,0x00);
	SSD1963_SPI_WriteReg(0x02,0x03);
	SSD1963_SPI_WriteReg(0x03,0xcc);
	SSD1963_SPI_WriteReg(0x04,0x46);
	SSD1963_SPI_WriteReg(0x05,0x0d);
	SSD1963_SPI_WriteReg(0x06,0x00);
	SSD1963_SPI_WriteReg(0x07,0x00);
	SSD1963_SPI_WriteReg(0x08,0x08);
	SSD1963_SPI_WriteReg(0x09,0x40);
	SSD1963_SPI_WriteReg(0x0a,0x88);
	SSD1963_SPI_WriteReg(0x0b,0x88);
	SSD1963_SPI_WriteReg(0x0c,0x30);
	SSD1963_SPI_WriteReg(0x0d,0x20);
	SSD1963_SPI_WriteReg(0x0e,0x6a);
	SSD1963_SPI_WriteReg(0x0f,0xa4);
	SSD1963_SPI_WriteReg(0x10,0x04);
	SSD1963_SPI_WriteReg(0x11,0x24);
	SSD1963_SPI_WriteReg(0x12,0x24);
	SSD1963_SPI_WriteReg(0x1e,0x00);
	SSD1963_SPI_WriteReg(0x20,0x00);				
	
	LCD_WriteCommand(0x29);	    /* Turn on display; show the image on display */	
	
	TFT_SET_BACKLIGHT(0xff);
}

/************************************************************************
 * 			FUNCTION:		LCD_SetArea
 ************************************************************************/

void TFT_SET_AREA(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
	LCD_WriteCommand(CMD_SET_COLUMN);
	LCD_WriteData(start_x>>8);
	LCD_WriteData(start_x);
	LCD_WriteData(end_x>>8);
	LCD_WriteData(end_x);
	
	LCD_WriteCommand(CMD_SET_PAGE);
	LCD_WriteData(start_y>>8);
	LCD_WriteData(start_y);
	LCD_WriteData(end_y>>8);
	LCD_WriteData(end_y);
}

/*****************************************************************************
 * 			FUNCTION:			TFT_SET_BACKLIGHT
 * 			DESCRIPTION:		0x00 (min)
 *                              0xff (max)
 *****************************************************************************/
void TFT_SET_BACKLIGHT(uint8_t power)
{
	LCD_WriteCommand(0xBE);
	LCD_WriteData(0x0E);			/* PWMF[7:0] = 14, PWM base freq = PLL/(256*(1+14))/256 = */ 
								    /* 122Hz for a PLL freq = 120MHz */
	LCD_WriteData(power);

	LCD_WriteData(0x01);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
}

/************************************************************************
 * 			FUNCTION:			TFT_CLEAR
 ************************************************************************/

void TFT_CLEAR(uint16_t Color)
{
	uint32_t index;
	
	TFT_SET_AREA(0,0,DISP_HOR_RESOLUTION -1 ,DISP_VER_RESOLUTION -1 );
	LCD_WriteCommand(CMD_WR_MEMSTART);
	for( index=0; index < DISP_HOR_RESOLUTION * DISP_VER_RESOLUTION; index++ )
	{
		LCD_WriteData(Color);
	}		 
}

/************************************************************************
 * 			FUNCTION:			TFT_GET_POINT
 ************************************************************************/

uint16_t TFT_GET_POINT(uint16_t Xpos,uint16_t Ypos)
{
	TFT_SET_AREA(Xpos,Ypos,Xpos,Ypos);
	LCD_WriteCommand(CMD_RD_MEMSTART);
	return LCD_ReadData();
}

/************************************************************************
 * 			FUNCTION:			TFT_SET_POINT
 ************************************************************************/

void TFT_SET_POINT(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos > DISP_HOR_RESOLUTION || Ypos > DISP_VER_RESOLUTION ) 
	{
		return;
	}
	TFT_SET_AREA(Xpos,Ypos,Xpos,Ypos);
	LCD_WriteCommand(CMD_WR_MEMSTART);
	LCD_WriteData(point);
}

/************************************************************************
 * 			FUNCTION:			PutCharacterNoBackground
 ************************************************************************/

void PutCharacterNoBackground( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> (7 - j)) & (0x01 == 0x01) )
            {
            	TFT_SET_POINT( Xpos + j, Ypos + i, charColor );
            }
        }
    }
}

/************************************************************************
 * 			FUNCTION:			PutCharacterInBackground
 ************************************************************************/

void PutCharacterInBackground( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> (7 - j)) & (0x01 == 0x01) )
            {
            	TFT_SET_POINT( Xpos + j, Ypos + i, charColor );
            }
            else
            {
            	TFT_SET_POINT( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}
