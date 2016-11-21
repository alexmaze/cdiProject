/**
****************************************************************

File		:	main.h
Author	:	@hiyangdong
Version	:	V1.0
date		:	2015-05-19 19:04:20
brief		:	main header

*****************************************************************
*/

#ifndef _CTASK_H_
#define _CTASK_H_


#include "main.h"
#include "usart.h"
#include "i2c.h"
#include "cLEDs.h"
#include "dataStruct.h"
#include "mI2C.h"

void cTaskList(void);
extern void cLEDs_Task(void);
extern void cI2C_Task1Fun(void);

#endif

