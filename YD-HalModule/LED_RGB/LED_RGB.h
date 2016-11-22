/**
****************************************************************

File		:	cLEDs.h
Author	:	@hiyangdong
Version	:	V1.0
date		:	2015-05-13
brief		:	information of the file

*****************************************************************
*/

#ifndef _CLEDS_H_
#define _CLEDS_H_

#include "stm32f1xx_hal.h"

typedef enum
{
  R = 0,
  G = 1,
  B = 2
}RGB;

typedef enum
{
  ON = 0,
  OFF
}LED_State;

#define R_GPIO  GPIOA
#define G_GPIO  GPIOA
#define B_GPIO  GPIOD
#define R_PIN GPIO_PIN_8
#define G_PIN GPIO_PIN_6
#define B_PIN GPIO_PIN_2


void RGB_Init(void);
void RGB_Write(RGB PinColor, LED_State LEDState);
void RGB_Toggle(RGB PinColor);


#endif
