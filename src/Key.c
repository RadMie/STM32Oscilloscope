#include "Key.h"

static BUTTON_T s_BtnUser;
static BUTTON_T s_BtnTamper;
static BUTTON_T s_BtnWakeUp;
static BUTTON_T s_BtnUp;
static BUTTON_T s_BtnDown;
static BUTTON_T s_BtnLeft;
static BUTTON_T s_BtnRight;
static BUTTON_T s_BtnOk;

static KEY_FIFO_T s_Key;

static uint8_t IsKeyDownUser(void) 		{if ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)  ) return 0; return 1;}
static uint8_t IsKeyDownTamper(void) 	{if ( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) ) return 0; return 1;}
static uint8_t IsKeyDownWakeUp(void) 	{if ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  ) return 1; return 0;}
static uint8_t IsKeyDownUp(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) ) return 0; return 1;}
static uint8_t IsKeyDownDown(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8)  ) return 0; return 1;}
static uint8_t IsKeyDownLeft(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) ) return 0; return 1;}
static uint8_t IsKeyDownRight(void) 	{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14) ) return 0; return 1;}
static uint8_t IsKeyDownOk(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7)  ) return 0; return 1;}

/*******************************************************************************
* Function Name  : KEY_GPIO_Configuration
*******************************************************************************/
static void KEY_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG, ENABLE); 						 
  /**
  *  JOY_UP -> PG11 , JOY_DOWN -> PG8 , JOY_LEFT -> PG13 , JOY_RIGHT -> PG14 , JOY_SEL -> PG7
  *  Tamper Button -> PC13 , Wakeup Button -> PA0 , User Button -> PA8
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : Key_Initializtion
*******************************************************************************/
void Key_Initializtion(void)
{
    KEY_GPIO_Configuration();

	s_Key.Read = 0;
	s_Key.Write = 0;

	s_BtnUser.IsKeyDownFunc = IsKeyDownUser;
	s_BtnUser.FilterTime = BUTTON_FILTER_TIME;
	s_BtnUser.LongTime = BUTTON_LONG_TIME;
	s_BtnUser.Count = s_BtnUser.FilterTime / 2;
	s_BtnUser.State = 0;
	s_BtnUser.KeyCodeDown = KEY_DOWN_USER;
	s_BtnUser.KeyCodeUp = KEY_UP_USER;
	s_BtnUser.KeyCodeLong = KEY_HOLD_USER;

	s_BtnTamper.IsKeyDownFunc = IsKeyDownTamper;
	s_BtnTamper.FilterTime = BUTTON_FILTER_TIME;
	s_BtnTamper.LongTime = 0;
	s_BtnTamper.Count = s_BtnTamper.FilterTime / 2;
	s_BtnTamper.State = 0;
	s_BtnTamper.KeyCodeDown = KEY_DOWN_TAMPER;
	s_BtnTamper.KeyCodeUp = 0;
	s_BtnTamper.KeyCodeLong = 0;

	s_BtnWakeUp.IsKeyDownFunc = IsKeyDownWakeUp;
	s_BtnWakeUp.FilterTime = BUTTON_FILTER_TIME;
	s_BtnWakeUp.LongTime = 0;
	s_BtnWakeUp.Count = s_BtnWakeUp.FilterTime / 2;
	s_BtnWakeUp.State = 0;
	s_BtnWakeUp.KeyCodeUp = 0;
	s_BtnWakeUp.KeyCodeDown = KEY_DOWN_WAKEUP;
	s_BtnWakeUp.KeyCodeLong = 0;

	s_BtnUp.IsKeyDownFunc = IsKeyDownUp;
	s_BtnUp.FilterTime = BUTTON_FILTER_TIME;
	s_BtnUp.LongTime = 0;
	s_BtnUp.Count = s_BtnUp.FilterTime / 2;
	s_BtnUp.State = 0;
	s_BtnUp.KeyCodeDown = KEY_DOWN_JOY_UP;
	s_BtnUp.KeyCodeUp = 0;
	s_BtnUp.KeyCodeLong = 0;

	s_BtnDown.IsKeyDownFunc = IsKeyDownDown;
	s_BtnDown.FilterTime = BUTTON_FILTER_TIME;
	s_BtnDown.LongTime = 0;
	s_BtnDown.Count = s_BtnDown.FilterTime / 2;
	s_BtnDown.State = 0;
	s_BtnDown.KeyCodeDown = KEY_DOWN_JOY_DOWN;
	s_BtnDown.KeyCodeUp = 0;
	s_BtnDown.KeyCodeLong = 0;

	s_BtnLeft.IsKeyDownFunc = IsKeyDownLeft;
	s_BtnLeft.FilterTime = BUTTON_FILTER_TIME;
	s_BtnLeft.LongTime = 0;
	s_BtnLeft.Count = s_BtnLeft.FilterTime / 2;
	s_BtnLeft.State = 0;
	s_BtnLeft.KeyCodeDown = KEY_DOWN_JOY_LEFT;
	s_BtnLeft.KeyCodeUp = 0;
	s_BtnLeft.KeyCodeLong = 0;

	s_BtnRight.IsKeyDownFunc = IsKeyDownRight;
	s_BtnRight.FilterTime = BUTTON_FILTER_TIME;
	s_BtnRight.LongTime = 0;
	s_BtnRight.Count = s_BtnRight.FilterTime / 2;
	s_BtnRight.State = 0;
	s_BtnRight.KeyCodeDown = KEY_DOWN_JOY_RIGHT;
	s_BtnRight.KeyCodeUp = 0;
	s_BtnRight.KeyCodeLong = 0;

	s_BtnOk.IsKeyDownFunc = IsKeyDownOk;
	s_BtnOk.FilterTime = BUTTON_FILTER_TIME;
	s_BtnOk.LongTime = 0;
	s_BtnOk.Count = s_BtnOk.FilterTime / 2;
	s_BtnOk.State = 0;
	s_BtnOk.KeyCodeDown = KEY_DOWN_JOY_OK;
	s_BtnOk.KeyCodeUp = 0;
	s_BtnOk.KeyCodeLong = 0;
}
/*******************************************************************************
* Function Name  : PutKey
* Input          : - _KeyCode: key
*******************************************************************************/
void PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}
/*******************************************************************************
* Function Name  : GetKey
*******************************************************************************/
uint8_t GetKey(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*******************************************************************************
* Function Name  : DetectButton
* Input          : - _pBtn
*******************************************************************************/
static void DetectButton(BUTTON_T *_pBtn)
{
	if (_pBtn->IsKeyDownFunc())
	{
		if (_pBtn->Count < _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		{
			_pBtn->Count++;
		}
		else
		{
			if (_pBtn->State == 0)
			{
				_pBtn->State = 1;

				if (_pBtn->KeyCodeDown > 0)
				{
					PutKey(_pBtn->KeyCodeDown);
				}
			}

			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{

					if (++_pBtn->LongCount == _pBtn->LongTime)
					{

						PutKey(_pBtn->KeyCodeLong);
					}
				}
			}
		}
	}
	else
	{
		if(_pBtn->Count > _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count != 0)
		{
			_pBtn->Count--;
		}
		else
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				if (_pBtn->KeyCodeUp > 0)
				{
					PutKey(_pBtn->KeyCodeUp);
				}
			}
		}
		_pBtn->LongCount = 0;
	}
}

/*******************************************************************************
* Function Name  : KeyPro
*******************************************************************************/
void KeyPro(void)
{
	DetectButton(&s_BtnUser);
	DetectButton(&s_BtnTamper);
	DetectButton(&s_BtnWakeUp);
	DetectButton(&s_BtnUp);
	DetectButton(&s_BtnDown);
	DetectButton(&s_BtnLeft);
	DetectButton(&s_BtnRight);
	DetectButton(&s_BtnOk);
}
