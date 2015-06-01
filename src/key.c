#include <key.h>
#include <stdbool.h>

/*******************************************************************************
* Function Name  : KEY_Configuration
*******************************************************************************/
void KEY_Configuration(void)
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

bool KeyJoyUpPressed() {

	 if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoyDownPressed() {

	 if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoyLeftPressed() {

	 if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoyRightPressed() {

	 if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoySelPressed() {

	 if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoyTamperPressed() {

	 if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoyWakeupPressed() {

	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {

		 return 0;
	 } else {

		 return 1;
	 }
}

bool KeyJoyUserPressed() {

	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)) {

		 return 0;
	 } else {

		 return 1;
	 }
}
