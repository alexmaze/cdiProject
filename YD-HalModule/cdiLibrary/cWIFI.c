/**
****************************************************************

File	:	cWIFI.c
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	code file

*****************************************************************
*/

#include "cWIFI.h"

/**
****************************************************************

Function：	cWIFI_Init
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	initialize the RGB pins

*****************************************************************
*/
void cWIFI_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

