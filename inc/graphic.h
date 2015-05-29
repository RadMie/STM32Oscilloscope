#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "stm32f10x.h"

void TFT_DRAW_LINE( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2 , uint16_t color );
void TFT_DRAW_RECTANGLE(uint16_t Xpos, uint16_t Ypos, uint16_t Width , uint16_t Height , uint16_t col );
void TFT_DRAW_FULL_RECTANGLE_IN_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t FrameColor, uint16_t Background_Color);
void TFT_DRAW_FULL_RECTANGLE_NO_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t color);
void TFT_DRAW_CIRCLE(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color);
void TFT_DISPLAY_TEXT_IN_BK(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t TextColor, uint16_t BackgroundColor);
void TFT_DISPLAY_TEXT_NO_BK(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t TextColor);

void bmp_display();

#endif
