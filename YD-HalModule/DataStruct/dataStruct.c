/*****************************************************************

File			:	YD-Module\dataStruct\dataStruct.c
Fuction		:	Power management chip
Author		:	@hiyangdong
Version		:	V1.0
Time			:	30 Nov. 2015

*****************************************************************/
#include "dataStruct.h"

//CRC_check

//address save

//address compare


/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
uint8_t bufferCmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength)
{
	BufferLength++;
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}


//		I2C_Data.size =  I2C_Data.data_read[6];
//		I2C_Data.cmd = I2C_Data.data_read[7];

//		memcpy(I2C_Data.data,I2C_Data.data_read + 8,6);
//		memcpy(I2C_Data.crc,I2C_Data.data_read + 14,2);
//		memmove(I2C_Data.data_send,I2C_Data.data_read,16);
void initI2CFrame(I2C_DataType I2C_Data)
{
	memset(I2C_Data.data_read,0,I2C_FRAME_SIZE);
	memset(I2C_Data.data_send,0,I2C_FRAME_SIZE);
	I2C_Data.status = HAL_OK;
	I2C_Data.flag = 0;
	I2C_Data.addr_hard = 0;
	I2C_Data.addr_soft = 0;
	I2C_Data.size = 0;
	I2C_Data.cmd = 0;
	memset(I2C_Data.data,0,I2C_DATA_SIZE);
	memset(I2C_Data.crc,0,I2C_CRC_SIZE);
}


void updateI2CFrame(I2C_DataType I2C_Data)
{
	I2C_Data.addr_hard = (I2C_Data.data_read[I2C_HARD_INDEX]<<8)
										 + (I2C_Data.data_read[I2C_HARD_INDEX+1]);
	I2C_Data.addr_soft = (I2C_Data.data_read[I2C_SOFT_INDEX]<<24)\
										 + (I2C_Data.data_read[I2C_SOFT_INDEX+1]<<16)\
										 + (I2C_Data.data_read[I2C_SOFT_INDEX+2]<<8)\
										 + (I2C_Data.data_read[I2C_SOFT_INDEX+3]);
	I2C_Data.size = I2C_Data.data_read[I2C_SIZE_INDEX];
	I2C_Data.cmd = I2C_Data.data_read[I2C_CMD_INDEX];
	memmove(I2C_Data.data,I2C_Data.data_read+I2C_DATA_INDEX,I2C_DATA_SIZE);
	memmove(I2C_Data.crc,I2C_Data.data_read+I2C_CRC_INDEX,I2C_CRC_SIZE);
}


uint16_t getI2CHardAddress(I2C_DataType I2C_Data)
{
	return I2C_Data.addr_hard;
}

uint32_t getI2CSoftAddress(I2C_DataType I2C_Data)
{
	return I2C_Data.addr_soft;
}

uint8_t checkI2CHardAddress(I2C_DataType I2C_Data)
{
	if(getI2CHardAddress(I2C_Data) == I2C_HARD_ADDR)
		return TRUE;
	else 
		return FALSE;
}

uint8_t checkI2CSoftAddress(I2C_DataType I2C_Data)
{
	if(getI2CSoftAddress(I2C_Data) == I2C_SOFT_ADDR)
		return TRUE;
	else 
		return FALSE;
}

uint8_t checkCRC(I2C_DataType I2C_Data)
{
	if((I2C_Data.crc[0] == 0x0D)&&(I2C_Data.crc[0] == 0x0A))
		return TRUE;
	else 
		return FALSE;
}

uint8_t executeCommand(I2C_DataType I2C_Data)
{
	return I2C_Data.flag;
}

uint8_t exeCmd(I2C_DataType I2C_Data)
{
	return executeCommand(I2C_Data);
}


