
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <string.h>
#include "cTask.h"

uint16_t gLength = 0;			//16kbytes,6bytes/frame =2730.666

void cTask_Init(void) {

	cLEDs_Init();
	cWIFI_Init();
//	HAL_GPIO_WritePin(WIFI_WorkMode_GPIO_Port,WIFI_WorkMode_Pin,GPIO_PIN_RESET);	//下拉：工作模式;
//	HAL_GPIO_WritePin(WIFI_PD_GPIO_Port,WIFI_PD_Pin,GPIO_PIN_SET);	//1）高电平工作;2）低电平模块供电关掉;
//	cWIFI_MspDeInit(&WIFI_UART);
}
void cTask_List(void) {

  cLEDs_Task();
	cWIFI_Task();
}

/*任务1： cLEDs  开始*/
osThreadId cLEDs_TaskHandle;
char cLEDs_status = 1;	//0：红色代表电源正常，1：绿色闪烁代表出错，2：蓝色代表需要重启或电源电压太低
void cLEDs_TaskFun(void const * argument) {

  while(1){
		if(cLEDs_status==0){
			cLEDs_Write(R,ON);
			cLEDs_Write(G,OFF);
			cLEDs_Write(B,OFF);
			osDelay(1000);
		}
		else if(cLEDs_status==1){
			cLEDs_Write(R,OFF);
			cLEDs_Toggle(G);
			cLEDs_Write(B,OFF);
			osDelay(1000);
		}
		else if(cLEDs_status==2){
			cLEDs_Write(R,OFF);
			cLEDs_Write(G,OFF);
			cLEDs_Toggle(B);
			osDelay(1000);
		}
		else{
			cLEDs_Write(R,OFF);
			cLEDs_Write(G,OFF);
			cLEDs_Write(B,OFF);
			osDelay(1000);
		}
  }
}

void cLEDs_Task(void) {
 
  osThreadDef(cLEDs_Handle, cLEDs_TaskFun, osPriorityNormal, 0, 128);
  cLEDs_TaskHandle = osThreadCreate(osThread(cLEDs_Handle), NULL);

}
/*任务1： cLEDs  结束*/

/*任务2： cWIFI  开始*/
osThreadId cWIFI_TaskHandle;
void cWIFI_TaskFun(void const * argument)
{
//	char cCmd [64];
//	cWIFI_Net_Mode_Choose(STA_AP);
//	memset(cCmd, 0x00, 256);
//	sprintf ( cCmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", TCP_SERVER_IPADDRESS,TCP_SERVER_PORT );	
//	uint8_t length = strlen(cCmd);
//	HAL_UART_Transmit(&WIFI_UART, cCmd, length, TX_TIMEOUT);
//	cWIFI_Rst();
//	cWIFI_CIPMUX();
//	cWIFI_CWLAP();
//	cWIFI_JoinAP();
	cWIFI_Cmd("AT+CIPMODE=0","OK",NULL,1000);
	osDelay(100);
	cWIFI_CWJAP(WIFI_SSID_NAME,WIFI_PASSWORD, 7800);
	osDelay(1000);
	cWIFI_Cmd("AT+CIPSTART=\"TCP\",\"alexyan.xyz\",4000","OK",NULL,2500);
	osDelay(1000);
	cWIFI_TCPSend(8000);
//	cWIFI_Cmd("AT+CIPSEND=67","OK",NULL,2500);
//	osDelay(100);
//	cWIFI_Cmd("GET /api/hardware/report/HW001 HTTP/1.1\r\nHost: alexyan.xyz:4000\r\n\r\n","SEND OK",NULL,5000);
	osDelay(1000);
}

void cWIFI_Task(void) {
 
  osThreadDef(cWIFI_Handle, cWIFI_TaskFun, osPriorityAboveNormal, 0, 256);
  cWIFI_TaskHandle = osThreadCreate(osThread(cWIFI_Handle), NULL);

}
/*任务2： cWIFI  结束*/


