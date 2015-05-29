#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define BUTTON_FILTER_TIME 	5
#define BUTTON_LONG_TIME 	100
#define KEY_FIFO_SIZE	    20

typedef struct
{
	uint8_t (*IsKeyDownFunc)(void);
	uint8_t Count;
	uint8_t FilterTime;
	uint16_t LongCount;
	uint16_t LongTime;
	uint8_t  State;
	uint8_t KeyCodeUp;
	uint8_t KeyCodeDown;
	uint8_t KeyCodeLong;
}BUTTON_T;

typedef enum
{
	KEY_NONE = 0,
	KEY_DOWN_USER,
	KEY_UP_USER,
	KEY_HOLD_USER,
	KEY_DOWN_WAKEUP,
	KEY_DOWN_TAMPER,
	KEY_DOWN_JOY_UP,
	KEY_DOWN_JOY_DOWN,
	KEY_DOWN_JOY_LEFT,
	KEY_DOWN_JOY_RIGHT,
	KEY_DOWN_JOY_OK
}KEY_ENUM;

typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];
	uint8_t Read;
	uint8_t Write;
}KEY_FIFO_T;

void KeyPro(void);
uint8_t GetKey(void);
void Key_Initializtion(void);
void PutKey(uint8_t _KeyCode);

#endif
