#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include <stdbool.h>

void KEY_Configuration(void);
bool KeyJoyUpPressed();
bool KeyJoyDownPressed();
bool KeyJoyLeftPressed();
bool KeyJoyRightPressed();
bool KeyJoySelPressed();
bool KeyJoyTamperPressed();
bool KeyJoyWakeupPressed();
bool KeyJoyUserPressed();

#endif
