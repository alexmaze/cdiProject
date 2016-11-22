/**
****************************************************************

File	:	sKey.c
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	information of the file

*****************************************************************
*/

#include "sKey.h"

/**
****************************************************************

Function£º	cKey_Init
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	initialize the RGB pins

*****************************************************************
*/
void sKey_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*Configure GPIO pin : PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
****************************************************************

Function£º	sKey_Read
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	write the RGB pins

*****************************************************************
*/
uint8_t Key_Read(void)
{
	
	uint8_t Pin_Value=0;
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)
	{
		HAL_Delay(10);
		Pin_Value = 1;
	}
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)
	else
		Pin_Value = 0;
	
	return Pin_Value;
	
}


