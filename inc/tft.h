#ifndef __TFT_H
#define __TFT_H

#include <math.h>
#include "stm32f10x.h"
#include "SSD1963_CMD.h"

#define USE_16BIT_PMP

/*********************************************************************
* Overview: Horizontal and vertical display resolution
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_RESOLUTION				320
#define DISP_VER_RESOLUTION				240

/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_PULSE_WIDTH		20    /* 20 */
#define DISP_HOR_BACK_PORCH			51	  /* 48	*/
#define DISP_HOR_FRONT_PORCH		20	  /* 20 */

/*********************************************************************
* Overview: Vertical synchronization timing in lines
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_VER_PULSE_WIDTH		2	  /* 2 */
#define DISP_VER_BACK_PORCH			12	  /* 16 */
#define DISP_VER_FRONT_PORCH		4	  /* 4 */

/*********************************************************************
* Definition for SPI interface for HIMAX 8238-A relevant to hardware 
* layout; Hardware dependent!
*********************************************************************/
#define GPIO3 3
#define GPIO2 2
#define GPIO1 1
#define GPIO0 0
#define LCD_RESET (1<<GPIO3)	   /* LCD Reset signal (Reset for display panel, NOT ssd1963) */
#define LCD_SPENA (1<<GPIO0)	   /* SPI EN signal */
#define LCD_SPCLK (1<<GPIO1)	   /* SPI CLK */
#define LCD_SPDAT (1<<GPIO2)	   /* SPI DATA */

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define Orange		   0xFBE1
#define loading		   0x6BF4

#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

void TFT_Init(void);
void LCD_WriteCommand(uint16_t cmd);
void LCD_WriteData(uint16_t data);
void TFT_SET_AREA(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
void TFT_SET_BACKLIGHT(uint8_t power);
void TFT_CLEAR(uint16_t Color);
uint16_t TFT_GET_POINT(uint16_t Xpos,uint16_t Ypos);
void TFT_SET_POINT(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void PutCharacterNoBackground( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor);
void PutCharacterInBackground( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void TFT_DRAW_LINE( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2 , uint16_t color );
void TFT_DRAW_RECTANGLE(uint16_t Xpos, uint16_t Ypos, uint16_t Width , uint16_t Height , uint16_t col );
void TFT_DRAW_FULL_RECTANGLE_NO_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t color);
void TFT_DRAW_CIRCLE(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color);
void TFT_DISPLAY_TEXT_IN_BK(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t TextColor, uint16_t BackgroundColor);
void TFT_DISPLAY_TEXT_NO_BK(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t TextColor);
void TFT_DRAW_FULL_RECTANGLE_IN_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t FrameColor, uint16_t Background_Color);

void TFT_SRAM_BUFFER_CLEAR(uint16_t Color);
void TFT_SRAM_BUFFER_SET_POINT(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void TFT_SRAM_BUFFER_DRAW_LINE( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2 , uint16_t color );
void TFT_SRAM_BUFFER_DRAW_RECTANGLE(uint16_t Xpos, uint16_t Ypos, uint16_t Width , uint16_t Height , uint16_t col);
void TFT_SRAM_BUFFER_PUT_CHARACTER_NO_BACKGROUND( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor );
void TFT_SRAM_BUFFER_PUT_CHARACTER_IN_BACKGROUND( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
uint16_t TFT_SRAM_BUFFER_GET_POINT(uint16_t Xpos,uint16_t Ypos);
void TFT_SRAM_BUFFER_DRAW_FULL_RECTANGLE_NO_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t color);
void TFT_SRAM_BUFFER_DRAW_CIRCLE(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color);
void TFT_SRAM_BUFFER_TO_LCD();

#endif
