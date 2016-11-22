/**
****************************************************************

File	:	cLEDs.c
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	information of the file

*****************************************************************
*/

#include "LED_RGB.h"

/**
****************************************************************

Function：	cLEDs_Init
Author	:	@hiyangdong
Version	:	V1.0
date		:	2015-05-13
brief		:	initialize the RGB pins

*****************************************************************
*/
void RGB_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = R_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(R_GPIO, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = G_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(G_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = B_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(B_GPIO, &GPIO_InitStruct);
	RGB_Write(R,OFF);
	RGB_Write(G,OFF);	
	RGB_Write(G,OFF);	
}

/**
****************************************************************

Function：	cLEDs_Write
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	write the RGB pins

*****************************************************************
*/
void RGB_Write(RGB PinColor, LED_State LEDState)
{
	GPIO_PinState PinState;
	if(LEDState==0)
	{
		PinState = GPIO_PIN_RESET;
	}
	else
		PinState = GPIO_PIN_SET;
	if(PinColor==R)
	{
		  HAL_GPIO_WritePin(R_GPIO,R_PIN,PinState);
	}
	else if(PinColor==G)
	{
		  HAL_GPIO_WritePin(G_GPIO,G_PIN,PinState);
	}	
	else if(PinColor==B)
	{
		  HAL_GPIO_WritePin(B_GPIO,B_PIN,PinState);
	}
}

/**
****************************************************************

Function：	cLEDs_Toggle
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	Toggle the RGB pins

*****************************************************************
*/
void RGB_Toggle(RGB PinColor)
{
	if(PinColor==R)
	{
		  HAL_GPIO_TogglePin(R_GPIO,R_PIN);
	}
	else if(PinColor==G)
	{
		  HAL_GPIO_TogglePin(G_GPIO,G_PIN);
	}	
	else if(PinColor==B)
	{
		  HAL_GPIO_TogglePin(B_GPIO,B_PIN);
	}
}
