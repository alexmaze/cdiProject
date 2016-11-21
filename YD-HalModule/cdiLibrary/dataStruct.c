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

/*
http://bbs.csdn.net/topics/380026484
int length = 12345678;
char *p = (char*)&length;
char send_buf[100];

memcpy(send_buf, p, sizeof(int));

// 解包时，取前4个字节为包长
int length = *(int*)send_buf;
*/


I2C_DataType update_I2CFrame(DeviceDataType *device)
{
	I2C_DataType i2c_data;
	i2c_data.addr_hard = device->type;
	i2c_data.addr_soft = device->address;
	i2c_data.size = I2C_FRAME_SIZE;
	i2c_data.cmd = device->command;
	i2c_data.value = device->value;
//	for(char i=0;i<6;i++)
//	{
//		i2c_data.data[i] = (device_value<<(5-i)*8);
//	}
	memcpy(i2c_data.data+2, &device->value, sizeof(device->value));
	
	memcpy(i2c_data.frame+I2C_HARD_INDEX, &i2c_data.addr_hard, sizeof(i2c_data.addr_hard));
	memcpy(i2c_data.frame+I2C_SOFT_INDEX, &i2c_data.addr_soft, sizeof(i2c_data.addr_soft));
	memcpy(i2c_data.frame+I2C_SIZE_INDEX, &i2c_data.size, sizeof(i2c_data.size));
	memcpy(i2c_data.frame+I2C_CMD_INDEX, &i2c_data.cmd, sizeof(i2c_data.cmd));
	memcpy(i2c_data.frame+I2C_DATA_INDEX, &i2c_data.data, sizeof(i2c_data.data));
	memcpy(i2c_data.frame+I2C_CRC_INDEX, &i2c_data.crc, sizeof(i2c_data.crc));
	return i2c_data;
}

void parse_I2CFrame(I2C_DataType I2C_Data)
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

void write_I2CFrame(uint16_t device_type, uint32_t device_address, uint8_t device_command, int32_t device_value)
{
	I2C_DataType i2c_data;
	i2c_data.addr_hard = device_type;
	i2c_data.addr_soft = device_address;
	i2c_data.size = I2C_FRAME_SIZE;
	i2c_data.cmd = device_command;
	i2c_data.value = device_value;
//	for(char i=0;i<6;i++)
//	{
//		i2c_data.data[i] = (device_value<<(5-i)*8);
//	}
	memcpy(i2c_data.data+2, &device_value, sizeof(device_value));
	
	memcpy(i2c_data.data_send+I2C_HARD_INDEX, &i2c_data.addr_hard, sizeof(i2c_data.addr_hard));
	memcpy(i2c_data.data_send+I2C_SOFT_INDEX, &i2c_data.addr_soft, sizeof(i2c_data.addr_soft));
	memcpy(i2c_data.data_send+I2C_SIZE_INDEX, &i2c_data.size, sizeof(i2c_data.size));
	memcpy(i2c_data.data_send+I2C_CMD_INDEX, &i2c_data.cmd, sizeof(device_address));
	memcpy(i2c_data.data_send+I2C_DATA_INDEX, &i2c_data.data, sizeof(i2c_data.data));
	memcpy(i2c_data.data_send+I2C_CRC_INDEX, &i2c_data.crc, sizeof(i2c_data.crc));
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


