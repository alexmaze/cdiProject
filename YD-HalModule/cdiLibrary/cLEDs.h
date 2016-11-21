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

#include "stm32l1xx_hal.h"

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

#define R_GPIO  RGB_R_GPIO_Port
#define G_GPIO  RGB_G_GPIO_Port
#define B_GPIO  RGB_B_GPIO_Port
#define R_PIN 	RGB_R_Pin
#define G_PIN 	RGB_G_Pin
#define B_PIN 	RGB_B_Pin

void cLEDs_Init(void);
void cLEDs_Write(RGB PinColor, LED_State LEDState);
void cLEDs_Toggle(RGB PinColor);

#endif
