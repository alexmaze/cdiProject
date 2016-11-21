/*****************************************************************

File			:	YD-Module\BLE_DA14580\BLE.h
Fuction		:	
Author		:	@hiyangdong
Version		:	V1.0
Time			:	30 Nov. 2015

*****************************************************************/

#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include "stm32l1xx_hal.h"
#include <string.h>

#define TRUE 1
#define FALSE 0
#define NULL 0

#define I2C_FRAME_SIZE			((uint8_t)16)
#define I2C_HARD_ADDR				((uint16_t)0x0020)
#define I2C_SOFT_ADDR				((uint32_t)0x01010101)

#define I2C_HARD_INDEX			((uint8_t)0)
#define I2C_SOFT_INDEX			((uint8_t)2)
#define I2C_SIZE_INDEX      ((uint8_t)6)
#define I2C_CMD_INDEX      	((uint8_t)7)
#define I2C_DATA_INDEX     	((uint8_t)8)
#define I2C_DATA_SIZE      	((uint8_t)6)
#define I2C_CRC_INDEX      	((uint8_t)14)
#define I2C_CRC_SIZE      	((uint8_t)2)

#define I2C_STATE_TIMEOUT						((uint16_t)10000)
#define I2C_TX_TIMEOUT          		((uint16_t)1000)
#define I2C_RX_TIMEOUT          		((uint16_t)1000)

#define COUNT(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

typedef enum
{
  READ	= 0x00,		/*Master		*/
  WRITE	=	0x01,		/*Slave			*/
}Command;	




typedef struct DataFormat
{
	uint8_t data_read[I2C_FRAME_SIZE];
	uint8_t data_send[I2C_FRAME_SIZE];
	HAL_StatusTypeDef status;
	uint8_t flag;
	uint16_t addr_hard;
	uint32_t addr_soft;
	uint8_t  size;
	uint8_t  cmd;
	uint8_t  data[6];
	uint16_t crc[2];	
}I2C_DataType;


uint8_t bufferCmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength);
void updateI2CFrame(I2C_DataType I2C_Data);
uint16_t getI2CHardAddress(I2C_DataType I2C_Data);
uint32_t getI2CSoftAddress(I2C_DataType I2C_Data);
uint8_t checkI2CHardAddress(I2C_DataType I2C_Data);
uint8_t checkI2CSoftAddress(I2C_DataType I2C_Data);
uint8_t checkCRC(I2C_DataType I2C_Data);
uint8_t executeCommand(I2C_DataType I2C_Data);
uint8_t exeCmd(I2C_DataType I2C_Data);
uint8_t checkI2CHardAddress(I2C_DataType I2C_Data);

#endif
