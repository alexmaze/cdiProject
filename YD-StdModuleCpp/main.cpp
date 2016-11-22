/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "WIFI_ESP.h"
#include <string.h>
#include <stdbool.h>
#include "mp3.h"
#include "LED_RGB.h"
//#include "wifi_config.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define Debug_UART huart1

#define WIFI_UART  huart3
#define TX_TIMEOUT				((uint32_t)1000)
#define RX_TIMEOUT 				((uint32_t)1000)
#define ERROR_MAX					((uint8_t)5)
#define RX_BUF_MAX_LEN		((uint16_t)1024)
char revArray[200];

//struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

HAL_StatusTypeDef Serial_PutByte(char param)
{
  /* May be timeouted... */
  if ( WIFI_UART.State == HAL_UART_STATE_TIMEOUT )
  {
    WIFI_UART.State = HAL_UART_STATE_READY;
  }
	
  return HAL_UART_Transmit(&WIFI_UART, &param, 1, TX_TIMEOUT);
}

//////////////////////////////////////////////////////////////////
HAL_StatusTypeDef Serial_strRead(char *pString,uint16_t len)
{

	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_UART_Receive(&WIFI_UART, pString, len, RX_TIMEOUT);
//	HAL_UART_Transmit(&Debug_UART, pString, len, 100);
	
  return status;
}
HAL_StatusTypeDef Serial_strWrite(char *pString)
{
  uint16_t length = 0;
	HAL_StatusTypeDef status = HAL_OK;
	
	length = strlen(pString);
	
	status = HAL_UART_Transmit(&WIFI_UART, pString, length, TX_TIMEOUT);
	status = HAL_UART_Transmit(&WIFI_UART, "\r\n", 2, 1);
//	Serial_strRead(revArray,200);
  return status;
}


HAL_StatusTypeDef AT_Commandx(char *pString)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint16_t length = 0;
	uint8_t state = 1;
	char pStringRead[32]="";
	char dataRead[20] = "Hello World";
	
	while(state >= 1)
	{
		status = Serial_strWrite(pString);
		switch(status)
		{
			case HAL_OK:
									Serial_strRead(pStringRead,6);
									if(status == HAL_OK)
									{
										if(strstr(dataRead,"OK"))
										{
											state = 0;
										}
										else
										{
											state++;
										}
									}
									else
									{
										state++;
									}			
			break;
			default:
									state++;
			break;
		}
		if(state>=ERROR_MAX)
		{
			state = 0;
		}
	}
	return status;
}


HAL_StatusTypeDef ESP8266_strWrite(char *pString)
{
  uint16_t length = 0;
	HAL_StatusTypeDef status = HAL_OK;
	
	length = strlen(pString);
	
	status = HAL_UART_Transmit(&WIFI_UART, pString, length, TX_TIMEOUT);
	status = HAL_UART_Transmit(&WIFI_UART, "\r\n", 2, 10);

  return status;
}

HAL_StatusTypeDef ESP8266_strRead(char *pString, uint32_t timeout)
{

	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_UART_Receive(&WIFI_UART, pString, RX_BUF_MAX_LEN, timeout);
//	HAL_UART_Transmit(&Debug_UART, pString, RX_BUF_MAX_LEN, 100);
	
  return status;
}
/*
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_Cmd (char * cmd, char * reply1, char * reply2, uint32_t timeout)
{    

	Serial_strWrite(cmd);

	if((reply1 == 0) &&(reply2 == 0))                      //不需要接收数据
		return true;
	
	ESP8266_strRead(revArray,timeout);                //延时
	  
	if((reply1 != 0) &&(reply2 != 0))
		return((bool) strstr(revArray, reply1) ||(bool) strstr(revArray, reply2)); 
 	
	else if(reply1 != 0)
		return((bool) strstr(revArray, reply1));
	
	else
		return((bool) strstr(revArray, reply2));
	

}

void ESP8266_Rst ( void )
{

//    ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_Delay(2000); 
}

void ESP8266_AT_Test ( void )
{

	while (!ESP8266_Cmd ("AT", "OK", NULL, 250))
    {
        ESP8266_Rst();
    }
	HAL_UART_Transmit(&Debug_UART, "AT is OK Lalala\n", 16, 100);
}

bool ESP8266_Cmd_Test (char * cmd, char * reply1, char * reply2)
{    

	Serial_strWrite(cmd);

	if((reply1 == 0) &&(reply2 == 0))                      //不需要接收数据
		return true;
	
	Serial_strRead(revArray,200);                 //延时
	
  
	if((reply1 != 0) &&(reply2 != 0))
		return((bool) strstr(revArray, reply1) ||(bool) strstr(revArray, reply2)); 
 	
	else if(reply1 != 0)
		return((bool) strstr(revArray, reply1));
	
	else
		return((bool) strstr(revArray, reply2));
	

}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	char str[] = "Hello     World";
	char str2[] = "W                    o";
	char dataRead[20] = "Hello World";
	HAL_StatusTypeDef status = HAL_OK;
	char *state;
	int i=0,j;
	float sinx=0;
	char dataMP3[7] = {0x7E,0x03,0x03,0x00,0xEF,0x00,0x00};
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
	RGB_Init();
//	HAL_UART_Transmit(&WIFI_UART, "ATE0\r\n", 6, 100);
//	AT_Command(AT);
//		Led led;
//    while(1)
//    {
//        led.led1On();
//        led.led2On();
//        led.ledDelay();
//        led.led1Off();
//        led.led2Off();
//        led.ledDelay();
//    }

//WIFI 模块的初始化程序
//		Serial_strWrite("AT");
//		HAL_Delay(1000);
//		Serial_strWrite("AT+CWMODE=3");
//		Serial_strWrite("AT+CIPMUX=0");
//		HAL_Delay(1000);
//		Serial_strWrite("AT+CIPMODE=0");
//		HAL_Delay(1000);
//		Serial_strWrite("AT+CWLAP");
//		HAL_Delay(3000);
//GPRS 模块

		ESP8266_AT_Test();
		
		if(ESP8266_Cmd("AT","OK",NULL,200)==1)
		{
			HAL_UART_Transmit(&Debug_UART, "AT is OK\n", 9, 100);
			HAL_Delay(1000);
		}

		if(ESP8266_Cmd("AT+CREG?","+CREG:","0,1",500)==1)
		{
			HAL_UART_Transmit(&Debug_UART, "AT+CREG is OK\n", 14, 100);
			HAL_Delay(1000);
		}
		
		while(ESP8266_Cmd("AT+CIPSTART=\"TCP\",\"183.230.40.33\",\"80\"","ALREADY CONNECT","CONNECT OK",2000)==1)
		{
			HAL_UART_Transmit(&Debug_UART, "AT+TCP is OK\n", 13, 100);
			HAL_Delay(1000);
//			ESP8266_Cmd("AT+CIPCLOSE",
//			HAL_Delay(1000);
		}

		
		Serial_strWrite("AT");
		HAL_Delay(1000);
		Serial_strWrite("AT+CREG?");
		HAL_Delay(1000);
		Serial_strWrite("AT");
		HAL_Delay(1000);
		
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

		
//		Serial_strWrite("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80");
//		HAL_Delay(500);
		Serial_strWrite("AT+CIPSTART=\"TCP\",\"183.230.40.33\",\"80\"");
		HAL_Delay(2000);
		
		
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)==GPIO_PIN_SET)
		{i=9;}
		else
		{i=0;}
//		i = rand()%1000;
		sprintf(str2,"{\"Hello\":%d}",i);
		
		j=135+strlen(str2);
		
		sprintf(str,"AT+CIPSEND=%d",j);
		Serial_strWrite(str);
		HAL_Delay(100);
		Serial_strWrite("POST /devices/2941240/datapoints?type=3 HTTP/1.1");
		HAL_Delay(10);
		Serial_strWrite("Host: api.heclouds.com");
		HAL_Delay(10);
		Serial_strWrite("api-key: 8zM4CpIIHvNhn9nXc2UxMEmmPfs=");
		HAL_Delay(10);
		j=strlen(str2);
		sprintf(str,"Content-Length: %d",j);
		Serial_strWrite(str);
		HAL_Delay(10);
		HAL_UART_Transmit(&WIFI_UART, "\r\n", 2, 1);
		HAL_Delay(10);
//		i = rand() % 10;

		
		HAL_UART_Transmit(&WIFI_UART, str2, 14, 100);
		
		HAL_UART_Transmit(&WIFI_UART, "\r\n", 2, 1);
//		Serial_strWrite("AT+CIPCLOSE");
//		HAL_Delay(100);
		
		
		
//		dataMP3[0] = 0x7E;
//		dataMP3[1] = 0x03;
//		dataMP3[2] = 0x03;
//		dataMP3[3] = 0x00;
//		dataMP3[4] = 0xEF;
//		
////{,0x03,0x03,0x00,0xEF,0x00,0x00};
//		HAL_UART_Transmit(&MP3_UART, dataMP3, 5, 100);
		RGB_Toggle(R);
		mp3_Vol(3);
		mp3_Song(1);
		HAL_Delay(1000);
		mp3_Next;
		HAL_Delay(1000);
		mp3_Pause;
		RGB_Toggle(R);
		HAL_Delay(1000);
		mp3_Play;
		HAL_Delay(100);
//		mp3_InsertSong(3);
		HAL_Delay(5000);

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
void MX_USART1_UART_Init(void)
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

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

}

/* USART3 init function */
void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);

}

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PC0 PC1 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 WIFI_RST_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|WIFI_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
