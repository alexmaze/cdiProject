/*****************************************************************

File			:	YD-Module\mp3_player\mp3.c
Fuction		:	Power management chip
Author		:	@hiyangdong
Version		:	V0.1
Time			:	1 Dec. 2015

*****************************************************************/
#include "mp3.h"

HAL_StatusTypeDef mp3_commamd(uint8_t cmd)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t cmdArray[5]={0x7E,0x03,0x00,0x00,0xEF};
	uint8_t revArray[4];
	
	cmdArray[2] = cmd;
	cmdArray[3] = cmdArray[1]^cmdArray[2];
	
	status = HAL_UART_Transmit(&MP3_UART, cmdArray, 5, 100);
	status = HAL_UART_Receive(&MP3_UART, revArray, 2, 200);
	if((status==HAL_OK)&&(revArray[0]=='O')&&(revArray[1]=='K'))
	{
		return status;	
	}
	
	return status;	
}

HAL_StatusTypeDef mp3_setting(uint8_t cmd, uint8_t para)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t cmdArray[6]={0x7E,0x04,0x00,0x00,0x00,0xEF};
	uint8_t revArray[4];
	
	cmdArray[2] = cmd;
	cmdArray[3] = para;
	cmdArray[4] = cmdArray[1]^cmdArray[2]^cmdArray[3];
	
	status = HAL_UART_Transmit(&MP3_UART, cmdArray, 6, 100);
	status = HAL_UART_Receive(&MP3_UART, revArray, 2, 200);
	if((status==HAL_OK)&&(revArray[0]=='O')&&(revArray[1]=='K'))
	{
		return status;	
	}
	
	return status;	
}

HAL_StatusTypeDef mp3_select(uint8_t cmd, uint8_t paraH, uint8_t paraL)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t cmdArray[7]={0x7E,0x05,0x00,0x00,0x00,0x00,0xEF};
	uint8_t revArray[4];
	
	cmdArray[2] = cmd;
	cmdArray[3] = paraH;
	cmdArray[4] = paraL;
	cmdArray[5] = cmdArray[1]^cmdArray[2]^cmdArray[3]^cmdArray[4];
	
	status = HAL_UART_Transmit(&MP3_UART, cmdArray, 7, 100);
	status = HAL_UART_Receive(&MP3_UART, revArray, 2, 200);
	if((status==HAL_OK)&&(revArray[0]=='O')&&(revArray[1]=='K'))
	{
		return status;	
	}
	
	return status;	
}

