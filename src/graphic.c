#include "graphic.h"
#include "tft.h"
#include <stdio.h>
#include <stdlib.h>

/************************************************************************
 * 			FUNCTION:		TFT_DRAW_LINE
 ************************************************************************/
void TFT_DRAW_LINE( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2 , uint16_t color ) {

	int CurrentX, CurrentY, Xinc, Yinc, Dx, Dy, TwoDx, TwoDy, TwoDxAccumulatedError, TwoDyAccumulatedError;

	Dx = (X2-X1);
	Dy = (Y2-Y1);

	TwoDx = Dx + Dx;
	TwoDy = Dy + Dy;

	CurrentX = X1;
	CurrentY = Y1;

	Xinc = 1;
	Yinc = 1;

	if(Dx < 0)
	  {
	  Xinc = -1;
	  Dx = -Dx;
	  TwoDx = -TwoDx;
	  }

	if (Dy < 0)
	  {
	  Yinc = -1;
	  Dy = -Dy;
	  TwoDy = -TwoDy;
	  }

	TFT_SET_POINT(X1,Y1,color);
	if ((Dx != 0) || (Dy != 0))
	  {
	  if (Dy <= Dx)
	    {
	    TwoDxAccumulatedError = 0;
	    do
		  {
	      CurrentX += Xinc;
	      TwoDxAccumulatedError += TwoDy;
	      if(TwoDxAccumulatedError > Dx)
	        {
	        CurrentY += Yinc;
	        TwoDxAccumulatedError -= TwoDx;
	        }
	       TFT_SET_POINT(CurrentX,CurrentY,color);
	       }while (CurrentX != X2);
	     }
	   else
	      {
	      TwoDyAccumulatedError = 0;
	      do
		    {
	        CurrentY += Yinc;
	        TwoDyAccumulatedError += TwoDx;
	        if(TwoDyAccumulatedError>Dy)
	          {
	          CurrentX += Xinc;
	          TwoDyAccumulatedError -= TwoDy;
	          }
	         TFT_SET_POINT(CurrentX,CurrentY, color);
	         }while (CurrentY != Y2);
	    }
	  }
}
/************************************************************************
 * 			FUNCTION:			TFT_DRAW_RECTANGLE
 ************************************************************************/
void TFT_DRAW_RECTANGLE(uint16_t Xpos, uint16_t Ypos, uint16_t Width , uint16_t Height , uint16_t col ) {

	TFT_DRAW_LINE( Xpos , Ypos, Xpos + Width, Ypos ,col );
	TFT_DRAW_LINE( Xpos , Ypos + Height, Xpos + Width, Ypos + Height ,col );
	TFT_DRAW_LINE( Xpos , Ypos, Xpos , Ypos + Height ,col );
	TFT_DRAW_LINE( Xpos + Width, Ypos, Xpos + Width, Ypos + Height ,col );
}
/************************************************************************
 * 			FUNCTION:			TFT_DRAW_FULL_RECTANGLE
 ************************************************************************/
void TFT_DRAW_FULL_RECTANGLE_IN_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t FrameColor, uint16_t Background_Color) {

	TFT_DRAW_RECTANGLE( Xpos , Ypos , Width , Height, FrameColor);

	TFT_SET_AREA(Xpos+1, Ypos+1, (Xpos+1)+(Width-2), (Ypos+1)+(Height-2));
	LCD_WriteCommand(CMD_WR_MEMSTART);

	uint32_t count;
	uint32_t pixel;
	pixel = ((Width)*(Height));

	for(count=0; count<pixel; count++) {

		LCD_WriteData(Background_Color);
	}
}
/************************************************************************
 * 			FUNCTION:		TFT_DRAW_FULL_RECTANGLE_NO_FRAME
 ************************************************************************/
void TFT_DRAW_FULL_RECTANGLE_NO_FRAME(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t color) {

	uint16_t i;
	for(i=0; i<Height; i++) {

		TFT_DRAW_LINE(Xpos, Ypos, Xpos + Width, Ypos ,color );
		Ypos++;
	}
}
/************************************************************************
 * 			FUNCTION:			TFT_DRAW_CIRCLE
 ************************************************************************/
void TFT_DRAW_CIRCLE(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color) {

	uint16_t x, y, xchange, ychange, radiusErr;
		x = radius;
		y = 0;
		xchange = 1 - 2 * radius;
		ychange = 1;
		radiusErr = 0;

	while(x >= y) {

		TFT_SET_POINT(Xpos+x, Ypos+y, color);
		TFT_SET_POINT(Xpos-x, Ypos+y, color);
		TFT_SET_POINT(Xpos-x, Ypos-y, color);
		TFT_SET_POINT(Xpos+x, Ypos-y, color);
		TFT_SET_POINT(Xpos+y, Ypos+x, color);
		TFT_SET_POINT(Xpos-y, Ypos+x, color);
		TFT_SET_POINT(Xpos-y, Ypos-x, color);
		TFT_SET_POINT(Xpos+y, Ypos-x, color);

		  y++;
		  radiusErr += ychange;
		  ychange += 2;

		if ( 2*radiusErr + xchange > 0 ) {
		    x--;
			radiusErr += xchange;
			xchange += 2;
		}
	}
}
/************************************************************************
 * 			FUNCTION:		TFT_DISPLAY_TEXT_IN_BK
 ************************************************************************/
void TFT_DISPLAY_TEXT_IN_BK(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t TextColor, uint16_t BackgroundColor) {

    uint8_t TempChar;
    do
    {
        TempChar = *str++;
        PutCharacterInBackground( Xpos, Ypos, TempChar, TextColor, BackgroundColor );
        if( Xpos < DISP_HOR_RESOLUTION - 8 )
        {
            Xpos += 8;
        }
        else if ( Ypos < DISP_VER_RESOLUTION - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}
/************************************************************************
 * 			FUNCTION:		TFT_DISPLAY_TEXT_NO_BK
 ************************************************************************/
void TFT_DISPLAY_TEXT_NO_BK(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t TextColor) {

    uint8_t TempChar;
    do
    {
        TempChar = *str++;
        PutCharacterNoBackground( Xpos, Ypos, TempChar, TextColor);
        if( Xpos < DISP_HOR_RESOLUTION - 8 )
        {
            Xpos += 8;
        }
        else if ( Ypos < DISP_VER_RESOLUTION - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}
