#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#include "cTask2.h"

//全局变量
char state = 0;
osThreadId Task2_Handle;

void Task1(void);

void cTaskList(void)
{
	Task1();
}

void key_led(void const * argument)
{
	DeviceDataType sKey1;
	sKey1.type = 0x11;
	sKey1.address = SLAVE_SOFT_ADDR;
	sKey1.command = 0x40;
	sKey1.value = 0;
	
	device_read(&sKey1);
	
	DeviceDataType sRGB1;
	sRGB1.type = 0x10;
	sRGB1.address = SLAVE_SOFT_ADDR+1;
	sRGB1.command = 0x40;
	sRGB1.value = sKey1.value;
	device_write(&sRGB1);
	osDelay(100);
}

void Task1(void)
{
  osThreadDef(task_Handle, key_led, osPriorityNormal, 0, 128);
  Task2_Handle = osThreadCreate(osThread(task_Handle), NULL);
}

