/**
****************************************************************

File	:	cWIFI.h
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	header file

*****************************************************************
*/
/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CWIFI_H_
#define _CWIFI_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "usart.h"
#include "httpPacket.h"
#include <stdbool.h>
	 
#define BUFFER_MAX     		((uint16_t)512)             //最大接收缓存字节数
#define TX_TIMEOUT				((uint32_t)1000)
#define RX_TIMEOUT 				((uint32_t)1000)
#define ERROR_MAX					((uint8_t)5)


typedef struct WIFIFormat
{
	char buffer[BUFFER_MAX];
	HAL_StatusTypeDef status;
	uint8_t flag;
	uint16_t addr_hard;
	uint32_t addr_soft;
	uint8_t  size;
	uint8_t  cmd;
	uint8_t  data[6];
	uint16_t crc[2];	
}WIFI_DataType;

typedef enum{
	WIFI_CLOSE = 0,
	WIFI_SLEEP = 1,
	WIFI_RESET = 2,
	WIFI_CWJAP = 3,
	WIFI_TCP = 4,
	WIFI_SEND = 5
} ENUM_WifiState;

typedef struct wifi_state
{
	
	uint8_t state;
	ENUM_WifiState stateStart;
	ENUM_WifiState stateCurrent;
	ENUM_WifiState stateExpect;
	
	uint8_t error;
	
}WifiState;

extern HttpPacket httpPacket;
extern WifiState  wifiState;
extern WIFI_DataType wifiData;
	 
extern char WIFI_SSID_NAME[];
extern char WIFI_PASSWORD[];
//extern char TCP_SERVER_IPADDRESS[];
extern char TCP_SERVER_IPADDRESS[];
extern char TCP_SERVER_PORT[];
	 
extern char TCP_CILENT_IPADDRESS[];
extern char TCP_CILENT_PORT[];

#define WIFI_UART  huart1

/* 数据类型 */
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID     = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

typedef enum
{
  ONx = 0,
  OFFx
}WIFI_powerMode;

void cWIFI_Init(void);
void cWIFI_MspInit(UART_HandleTypeDef* uartHandle);
void cWIFI_MspDeInit(UART_HandleTypeDef* uartHandle);
void cWIFI_PD ( WIFI_powerMode mode );
HAL_StatusTypeDef cWIFI_strWrite(char *pString);
HAL_StatusTypeDef cWIFI_strRead(char *pString, uint32_t timeout);
void cWIFI_Rst ( void );
void cWIFI_AT_Test ( void );
void cWIFI_ATE ( void );
void cWIFI_CIPMUX ( void );
void cWIFI_CWLAP ( void );
void cWIFI_JoinAP ( void );
bool cWIFI_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode );
bool cWIFI_STAJoinTCP ( char * ip, char * port );
void cWIFI_CIPCLOSE ( void );
void cWIFI_JoinTCP ( void );
void cWIFI_CIPSEND ( void );
bool cWIFI_StrSend (char * pStr);
void cWIFI_Packet(OPS_TYPE operType, char* jsonData);
bool cWIFI_Cmd (char * cmd, char * reply1, char * reply2, uint32_t timeout);















//#define     ESP8266_Usart( fmt, ... )           USART2_printf ( USART2, fmt, ##__VA_ARGS__ ) 
//#define     PC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )

//#define     ESP8266_CH_HIGH_LEVEL()             GPIO_SetBits( GPIOA, GPIO_Pin_0 )
//#define     ESP8266_CH_LOW_LEVEL()              GPIO_ResetBits( GPIOA, GPIO_Pin_0 )

//#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits( GPIOA, GPIO_Pin_1 )
//#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits( GPIOA, GPIO_Pin_1 )


//void        ESP8266_Choose                      ( FunctionalState enumChoose );
//void        ESP8266_Rst                         ( void );
//void        ESP8266_AT_Test                     ( void );
//bool        ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
//bool        ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
//bool        ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
//bool        ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, char * enunPsdMode );
//bool        ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
//bool        ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
//bool        ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
//bool        ESP8266_TransparentTransmission               ( void );
//bool        ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
//char *      ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );

//void        ESP8266_STA_TCP_Client              ( void );
//void        ESP8266_AP_TCP_Server               ( void );
//void        ESP8266_StaTcpClient_ApTcpServer    ( void );
//void        Linktcp_LED_ON                      (void);
//void        LED_all_OFF                         (void);


#endif    /* __WIFI_CONFIG_H */


#ifdef __cplusplus
}
#endif 



