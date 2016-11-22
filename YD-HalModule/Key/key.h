/**
****************************************************************

File	:	sKey.h
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	information of the file

*****************************************************************
*/

#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f1xx_hal.h"

void Key_Init(void);
uint8_t Key_Read(void);
//void Key_Write(RGB PinColor, LED_State LEDState);
//void Key_Toggle(RGB PinColor);


#endif

/**
****************************************************************

File		:	sKey.h
Author	:	@hiyangdong
Version	:	V1.0
date		:	2015-05-13
brief		:	application of the file

*****************************************************************
*/
/*
					case 0x10:sKey_Write(R,ON);
										break;
					case 0x11:sKey_Write(R,OFF);
										break;
					case 0x12:sKey_Toggle(R);
										break;
					case 0x13:sKey_Write(G,ON);
										break;
					case 0x14:sKey_Write(G,OFF);
										break;
					case 0x15:sKey_Toggle(G);
										break;
					case 0x16:sKey_Write(B,ON);
										break;
					case 0x17:sKey_Write(B,OFF);
										break;
					case 0x18:sKey_Toggle(B);
										break;
*/
