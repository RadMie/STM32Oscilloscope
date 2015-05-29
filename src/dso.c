#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "dso.h"
#include "Key.h"
#include "tft.h"
#include "graphic.h"
#include "systick.h"

#define DSO_VER	 "V0.1"
#define ATT_COUNT	6	  /* g_DSO.Ch1Attenuation = 23 : (1) 28 === 1V  (1:1) */

/* g_DSO.TimeBaseId */
const uint32_t g_TimeTable[] =
{
	20, 	50, 	100, 
	200,	500,	1000,
	2000,	5000,	10000,
	20000,	50000,	100000,
	200000,	500000,	1000000		
};

const float g_AttTable[ATT_COUNT][2] =
{
	/* 1:1 */
	{327.6,	5000},
	{130.8,	2000},
	{64.8,	1000},
	{32.4,	500},
	{12,	200},
	{6,	    100},
};

DSO    g_DSO;

/*******************************************************************************
* Function Name  : DSO_Initializtion
*******************************************************************************/
void DSO_Initializtion(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef	ADC_InitStructure;	
  GPIO_InitTypeDef GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* DMA1_1 CH1 */
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch1Buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* DMA2_5 CH2 */
  /* Enable DMA2 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  DMA_DeInit(DMA2_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch2Buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, ENABLE);

  /* ADC1  */
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2);     /* ADCCLK = PCLK2/4 = 18MHz */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channels configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);

  /* Enable ADC1 external trigger conversion */
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  /* ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* ʹ ADC1  */
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));

  /*  ADC1  */
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));

  /* ADC3  */
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC1 regular channels configuration */
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_1Cycles5);

  /* Enable ADC3 external trigger conversion */
  ADC_ExternalTrigConvCmd(ADC3, ENABLE);

  /* ADC3 */
  ADC_Cmd(ADC3, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /*  ADC3 */
  ADC_ResetCalibration(ADC3);
  while(ADC_GetResetCalibrationStatus(ADC3));

  /*  ADC3 */
  ADC_StartCalibration(ADC3);
  while(ADC_GetCalibrationStatus(ADC3));

  /* TIM1 CC1 */
  DSO_SampleRate(g_DSO.SampleFreq);
}

/*******************************************************************************
* Function Name  : DSO_SampleRate
*******************************************************************************/
void DSO_SampleRate(uint32_t freq)
{
  uint16_t Period;	
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  TIM_Cmd(TIM1, DISABLE);

  //g_DSO.SampleRate = freq;
	
  /* period = 72000000 / freq ; 1200 == 60KHz */
  Period = 72000000 / freq;		

  /* Enable TIM1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = Period;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}
/*******************************************************************************
* Function Name  : Display_Frame
*******************************************************************************/
void Display_Frame(void)
{
  uint16_t x, y;

  //TFT_CLEAR(GUI_BLACK);
  //GUI_ClearRect(9 + 1, 19 + 1, 9 + 302 - 1 , 19 + 202 -1 );
  //TFT_DRAW_FULL_RECTANGLE_NO_FRAME(9+1, 19+1, 9+302-1, 19+202-1, GUI_BLACK);
  TFT_DRAW_FULL_RECTANGLE_IN_FRAME(9, 19, 302, 202, GUI_WHITE, GUI_BLACK);
 // TFT_DRAW_RECTANGLE(9, 19, 302 ,202 , GUI_WHITE);
						   
  for (x = 0; x < 13; x++)
  {
	 for (y = 0; y < 41; y++)
	 {
		 TFT_SET_POINT(10 + (x * 25), 20 + (y * 5), GUI_WHITE);
	 }
  }
  for (y = 0; y < 9; y++)
  {
	 for (x = 0; x < 61; x++)
	 {
		 TFT_SET_POINT(10 + (x * 5), 20 + (y * 25), GUI_WHITE);
	 }
  }
  for (y = 0; y < 41; y++)
  {	 
	 TFT_SET_POINT(9 + (6 * 25), 20 + (y * 5), GUI_WHITE);
	 TFT_SET_POINT(11 + (6 * 25), 20 + (y * 5), GUI_WHITE);
  }
  for (x = 0; x < 61; x++)
  {	 
	 TFT_SET_POINT(10 + (x * 5), 19 + (4 * 25), GUI_WHITE);
	 TFT_SET_POINT(10 + (x * 5), 21 + (4 * 25), GUI_WHITE);
  }
}

/*******************************************************************************
* Function Name  : LCD_DrawPoints
*******************************************************************************/
void LCD_DrawPoints( uint16_t *x, uint16_t *y, uint16_t Size, uint16_t color )
{
	uint16_t i;

	for (i = 0 ; i < Size - 1; i++)
	{
		TFT_DRAW_LINE( x[i], y[i], x[i + 1], y[i + 1] ,color );
	}
}

/*******************************************************************************
* Function Name  : Display_CH_Info
*******************************************************************************/	
void Display_CH_Info(void)
{
	char buf[32];

	TFT_DISPLAY_TEXT_NO_BK(10, 2, DSO_VER, GUI_WHITE);

	if (g_DSO.Ch1DC == 1)
	{
		strcpy(buf, "CH1 DC ");
	}
	else
	{
		strcpy(buf, "CH1 DC ");
	}

	if (g_DSO.Ch1VScale >= 1000)
	{
		sprintf(&buf[7], "%d.00V", g_DSO.Ch1VScale / 1000);
	}
	else
	{
		sprintf(&buf[7], "%dmV", g_DSO.Ch1VScale);
	}

	if (g_DSO.ActiveCH == 1)
	{
		TFT_DISPLAY_TEXT_IN_BK(10, 224, buf, GUI_WHITE,GUI_MAGENTA);
	}
	else
	{
		TFT_DISPLAY_TEXT_IN_BK(10, 224, buf, GUI_WHITE,GUI_BLACK);
	}

	if (g_DSO.Ch2DC == 1)
	{
		strcpy(buf, "CH2 DC ");
	}
	else
	{
		strcpy(buf, "CH2 DC ");
	}

	if (g_DSO.Ch2VScale >= 1000)
	{
		sprintf(&buf[7], "%d.00V", g_DSO.Ch2VScale / 1000);
	}
	else
	{
		sprintf(&buf[7], "%dmV", g_DSO.Ch2VScale);
	}
	if (g_DSO.ActiveCH == 2)
	{
		TFT_DISPLAY_TEXT_IN_BK(120, 224, buf, GUI_WHITE,GUI_MAGENTA);
	}
	else
	{
		TFT_DISPLAY_TEXT_IN_BK(120, 224, buf, GUI_WHITE,GUI_BLACK);
	}

	if (g_DSO.TimeBase < 1000)
	{
		sprintf(buf, "Time %3dus", (int)g_DSO.TimeBase);
	}
	else if (g_DSO.TimeBase < 1000000)
	{
		sprintf(buf, "Time %3dms", (int)(g_DSO.TimeBase / 1000));
	}
	else
	{
		sprintf(buf, "Time %3ds ", (int)(g_DSO.TimeBase / 1000000));
	}
	TFT_DISPLAY_TEXT_IN_BK(230, 224, buf, GUI_WHITE,GUI_BLACK);

	if (g_DSO.AdjustMode == 1)
	{
		TFT_DISPLAY_TEXT_IN_BK(180, 2, "M:1", GUI_WHITE,GUI_BLACK);
	}
	else
	{
		TFT_DISPLAY_TEXT_IN_BK(180, 2, "M:0", GUI_WHITE,GUI_BLACK);
	}

	if(g_DSO.HoldEn == 1) {

		TFT_DISPLAY_TEXT_IN_BK(250, 2, "H:1", GUI_WHITE,GUI_BLACK);
	} else {

		TFT_DISPLAY_TEXT_IN_BK(250, 2, "H:0", GUI_WHITE,GUI_BLACK);
	}

	sprintf(buf, "Freq:%7dHz",	g_DSO.SampleFreq);
	TFT_DISPLAY_TEXT_IN_BK(55, 2, buf, GUI_WHITE,GUI_BLACK);
}

/*******************************************************************************
* Function Name  : DSO_Configuration
*******************************************************************************/
void DSO_Configuration(void)
{
  g_DSO.Ch1Attenuation = 23;
  g_DSO.Ch2Attenuation = 23;
  g_DSO.Ch1VScale = 1000;
  g_DSO.Ch2VScale = 1000;

  g_DSO.Ch1VOffset = 70;
  g_DSO.Ch2VOffset = 170;

  g_DSO.ActiveCH = 1;
  g_DSO.AdjustMode = 1;

  g_DSO.HoldEn = 0;

  g_DSO.TimeBaseId = 0;									 
  g_DSO.TimeBase = 	g_TimeTable[g_DSO.TimeBaseId];
  g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

  g_DSO.Ch1AttId = 2;
  g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];
  g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];

  g_DSO.Ch2AttId = 2;
  g_DSO.Ch2Attenuation = g_AttTable[g_DSO.Ch2AttId][0];
  g_DSO.Ch2VScale =  g_AttTable[g_DSO.Ch2AttId][1];
}

/*******************************************************************************
* Function Name  : Inc_SampleFreq
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
*******************************************************************************/
void Inc_SampleFreq(void)
{
  if (g_DSO.TimeBaseId < (sizeof(g_TimeTable) / 4) - 1)
  {
	 g_DSO.TimeBaseId++;
  }
													 
  g_DSO.TimeBase = 	g_TimeTable[g_DSO.TimeBaseId];
  g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

  DSO_SampleRate(g_DSO.SampleFreq);
}

/*******************************************************************************
* Function Name  : Dec_SampleFreq
* Description    : 1-2-5
*******************************************************************************/
void Dec_SampleFreq(void)
{
  if (g_DSO.TimeBaseId > 0)
  {
	 g_DSO.TimeBaseId--;
  }													 
  g_DSO.TimeBase = g_TimeTable[g_DSO.TimeBaseId];
  g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

  DSO_SampleRate(g_DSO.SampleFreq);
}

/*******************************************************************************
* Function Name  : Adjust_DSO
* Description    : 1-2-5
*******************************************************************************/
void Adjust_DSO(uint8_t ch, uint8_t mode)
{

  if (ch == 1)
  {
	 if (mode == 0)
	 {
		 if (g_DSO.Ch1AttId > 0)
		 {
			 g_DSO.Ch1AttId--;
		 }
	 }
	 else
	 {
		 if (g_DSO.Ch1AttId < ATT_COUNT - 1)
		 {
			 g_DSO.Ch1AttId++;
		 }
	 }
	 g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];
	 g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];
  }
  else if (ch == 2)
  {
	 if (mode == 0)
	 {
		 if (g_DSO.Ch2AttId > 0)
		 {
			 g_DSO.Ch2AttId--;
		 }
	 }
	 else
	 {
		 if (g_DSO.Ch2AttId < ATT_COUNT - 1)
		 {
			 g_DSO.Ch2AttId++;
		 }
	 }
	 g_DSO.Ch2Attenuation = g_AttTable[g_DSO.Ch2AttId][0];
	 g_DSO.Ch2VScale =  g_AttTable[g_DSO.Ch2AttId][1];
  }
}

/*******************************************************************************
* Function Name  : Stop_ADC
*******************************************************************************/
void Stop_ADC(void)
{
  TIM_Cmd(TIM1, DISABLE);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, DISABLE);
	
  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, DISABLE);
}

/*******************************************************************************
* Function Name  : Start_ADC
*******************************************************************************/
void Start_ADC(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch1Buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  DMA_DeInit(DMA2_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch2Buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, ENABLE);

  TIM_Cmd(TIM1, ENABLE);
}

/*******************************************************************************
* Function Name  : DSO_Run
*******************************************************************************/
void DSO_Run(void)
{
  uint8_t key;

  //DSO_Configuration();
  //DSO_Initializtion();	 /* ADC,TIM,DMA */
  delay_ms(100);
  for(;;)
  {
	 KeyPro();
	 key = GetKey();
	 if ( g_DSO.HoldEn == 0 )
	 	{
	 		Stop_ADC();
	 		Display_DSO();
	 		Start_ADC();
	 	}
	 delay_ms(20);
	 if (key > 0)
	 {
		switch (key)
		{
		   case  KEY_DOWN_TAMPER:
				 if (g_DSO.ActiveCH == 1)
				 {
					 g_DSO.ActiveCH = 2;
				 }
				 else
				 {
					 g_DSO.ActiveCH = 1;
				 }
				 if (g_DSO.HoldEn == 1)
				 	{
				 	   Display_DSO();
				 	}
				 break;

		   case  KEY_DOWN_WAKEUP:
				 break;

		   case  KEY_DOWN_USER:
				 if ( g_DSO.HoldEn == 0 )
				 {
					 g_DSO.HoldEn = 1;
					 g_DSO.TimeBaseIdHold = g_DSO.TimeBaseId;	
                     Stop_ADC();
				 }
				 else
				 {
					 g_DSO.HoldEn = 0;
					 Start_ADC();				
				 }
				 if (g_DSO.HoldEn == 1)
				 	{
				 	   Display_DSO();
				 	}
				 break;

		   case  KEY_DOWN_JOY_LEFT:
				 if (g_DSO.HoldEn == 0)
				 {
					 Dec_SampleFreq();
					if (g_DSO.HoldEn == 1)
					 	{
					 	   Display_DSO();
					 	}
				 }
				 else
				 {

				 }
				 break;

		   case  KEY_DOWN_JOY_RIGHT:
				 if (g_DSO.HoldEn == 0)
				 {
					 Inc_SampleFreq();
					 if (g_DSO.HoldEn == 1)
					 	{
					 	   Display_DSO();
					 	}
				 }
				 else
				 {

				 }
				 break;

		   case  KEY_DOWN_JOY_OK:
				 if (g_DSO.AdjustMode == 0)
				 {
					 g_DSO.AdjustMode = 1;
				 }
				 else
				 {
					 g_DSO.AdjustMode = 0;
				 }
				 if (g_DSO.HoldEn == 1)
				 	{
				 	   Display_DSO();
				 	}
				 break;

		   case  KEY_DOWN_JOY_UP:
				 if (g_DSO.ActiveCH == 1)
				 {
					if (g_DSO.AdjustMode == 0)
					{
						Adjust_DSO(1, 1);
					}
					else
					{
						g_DSO.Ch1VOffset -= 5;
					}
				 }
				 else
				 {
					 if (g_DSO.AdjustMode == 0)
					 {
						Adjust_DSO(2, 1);
					 }
					 else
					 {
						g_DSO.Ch2VOffset -= 5;
					 }
				 }
				 if (g_DSO.HoldEn == 1)
				 	{
				 	   Display_DSO();
				 	}
				 break;

		   case  KEY_DOWN_JOY_DOWN:
				 if (g_DSO.ActiveCH == 1)
				 {
					if (g_DSO.AdjustMode == 0)
					{
						Adjust_DSO(1, 0);
					}
					else
					{
						g_DSO.Ch1VOffset += 5;
					}
				 }
				 else
				 {
					if (g_DSO.AdjustMode == 0)
					{
							Adjust_DSO(2, 0);
					}
					else
					{
							g_DSO.Ch2VOffset += 5;
					}
				  }
				 if (g_DSO.HoldEn == 1)
				 	{
				 	   Display_DSO();
				 	}
				 break;

			default:
				 break;
		 }
	  }
  }
}

/*******************************************************************************
* Function Name  : Display_DSO
*******************************************************************************/		
void Display_DSO(void)
{
	Display_CH_Info();
    _DrawDSO();
}

/*******************************************************************************
* Function Name  : _Draw
*******************************************************************************/
void _Draw(void * p)
{
  PARAM * pParam = (PARAM *)p;
  Display_Frame();

  LCD_DrawPoints( (void*)pParam->CH1x, (void*)pParam->CH1y, 300, GUI_LIGHTYELLOW);
  LCD_DrawPoints( (void*)pParam->CH2x, (void*)pParam->CH2y, 300, GUI_LIGHTMAGENTA);
}
/*******************************************************************************
* Function Name  : _DrawDSO
*******************************************************************************/
void _DrawDSO(void) 
{
  PARAM Param;	
  int16_t index;	
  int16_t iTemp;

  if (g_DSO.BufUsed == 0)
  {
	 g_DSO.BufUsed = 1;
  }
  else
  {
	 g_DSO.BufUsed = 0;
  }

  if (g_DSO.BufUsed == 0)
  {
	 Param.CH1x = g_DSO.xCh1Buf1;
	 Param.CH1y = g_DSO.yCh1Buf1;
	 Param.CH2x = g_DSO.xCh2Buf1;
	 Param.CH2y = g_DSO.yCh2Buf1;
  }
  else
  {
	 Param.CH1x = g_DSO.xCh1Buf2;
	 Param.CH1y = g_DSO.yCh1Buf2;
	 Param.CH2x = g_DSO.xCh2Buf2;
	 Param.CH2y = g_DSO.yCh2Buf2;
  }

  for (index = 0; index < 300; index++)
  {										 
	 Param.CH1x[index] = Param.CH2x[index] = 10 + index;
	 iTemp = g_DSO.Ch1VOffset - (int16_t)(( g_DSO.Ch1Buf[index + 1] - 2070 ) * 10) / g_DSO.Ch1Attenuation;

	 if (iTemp > 220){
		 iTemp = 220;
	 }
	 else if (iTemp < 20){
		 iTemp = 20;
	 }
	 Param.CH1y[index] = iTemp;

	 iTemp = g_DSO.Ch2VOffset - (int16_t)(( g_DSO.Ch2Buf[index + 1] - 2070 ) * 10) / g_DSO.Ch2Attenuation;

	 if (iTemp > 220){
		 iTemp = 220;
	 }
	 else if (iTemp < 20){
		 iTemp = 20;
	 }
	 Param.CH2y[index] = iTemp;

  }
  _Draw(&Param);
}
