#ifndef __DSO_H 
#define __DSO_H

#include "stm32f10x.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC2_DR_Address    ((uint32_t)0x4001284C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)

#define	SAMPLE_COUNT	   (4 * 1024)
	
typedef struct
{
  uint16_t Ch1Buf[SAMPLE_COUNT];
  uint16_t Ch2Buf[SAMPLE_COUNT];

  uint32_t TimeBaseIdHold;

  uint32_t TimeBaseId;
  uint32_t SampleFreq;
  uint32_t TimeBase;

  uint8_t  Ch1AttId;
  uint8_t  Ch2AttId;
  float    Ch1Attenuation;
  float    Ch2Attenuation;
  uint16_t Ch1VScale;
  uint16_t Ch2VScale;

  uint32_t TriggerLevel;
  uint32_t TriggerUpEdge;

  uint8_t Ch1DC;
  uint8_t Ch2DC;

  int16_t Ch1VOffset;
  int16_t Ch2VOffset;

  uint8_t ActiveCH;
  uint8_t AdjustMode;

  int16_t xCh1Buf1[310];
  int16_t yCh1Buf1[310];
  int16_t xCh1Buf2[310];
  int16_t yCh1Buf2[310];

  int16_t xCh2Buf1[310];
  int16_t yCh2Buf1[310];
  int16_t xCh2Buf2[310];
  int16_t yCh2Buf2[310];
  uint8_t BufUsed;

  uint8_t HoldEn;
}
DSO;

typedef struct 
{
   int16_t * CH1x;
   int16_t * CH1y;
   int16_t * CH2x;
   int16_t * CH2y;
} 
PARAM;
	
void DSO_SampleRate(uint32_t freq);
void DSO_Initializtion(void);
void Display_Frame(void);
void Display_CH1_Wave(void);
void Display_CH2_Wave(void);
void Display_CH_Info(void);
void Display_DSO(void);
void DSO_Configuration(void);
void Inc_SampleFreq(void);
void Dec_SampleFreq(void);
void Adjust_DSO(uint8_t ch, uint8_t mode);
void Stop_ADC(void);
void Start_ADC(void);
void DSO_Run(void);
void _DrawDSO(void) ;
void _Draw(void * p);

#endif
