#ifndef _WIFI_CONFIG_H_
#define	_WIFI_CONFIG_H_


#include "stm32f1xx_hal.h"
#include <stdbool.h>

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数
//char TCP_SERVER_IPADDRESS[] = "cdi.tongji.edu.cn";
char TCP_SERVER_IPADDRESS[] = "183.230.40.33";
char TCP_SERVER_PORT[] = "80";

char WIFI_SSID[] = "623network5_8GHz";
char WIFI_PWD[]  = "6958949599";

//char WIFI_SSID[] = "Dong.Yang";
//char WIFI_PWD[]  = "12341234";

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




extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  RX_BUF[ RX_BUF_MAX_LEN ];

  union {
    __IO uint16_t InfAll;
    struct {
		  __IO uint16_t FramLength       :15;                               // 14:0 
		  __IO uint16_t FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} ESP8266_Frame;


typedef struct wifi_state
{
	
	uint8_t state;
	uint8_t stateCurrent;
	uint8_t stateExpect;
	
	uint8_t error;
	
}WifiState;

//extern struct  STRUCT_USART1_1_Fram                                  //串口数据帧的处理结构体
//{
//	char  Data_RX_BUF[ RX_BUF_MAX_LEN ];
//	
//  union {
//    __IO uint16_t InfAll;
//    struct {
//		  __IO uint16_t FramLength       :15;                               // 14:0 
//		  __IO uint16_t FramFinishFlag   :1;                                // 15 
//	  } InfBit;
//  }; 
//	
//} strPc1_1_Fram_Record, str1_1esp8266;


////* 函数 */
//#define WiFi_RST_INIT                  HAL_GPIO_WritePinGPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                //WiFi使用的ES引脚初始化函数，推挽输出 
//#define WiFi_led_INIT				   LED_GPIO_Config			  //LED时序显示，
//#define WiFi_USART1_INIT               USART1_Config              //WiFi使用的串口1初始化函数，波特率9600
//#define WiFi_USART2_INIT               USART2_Config              //WiFi使用的串口2初始化函数，波特率9600 
//#define WiFi_NVIC_INIT                 NVIC_Configuration         //NVIC中断配置


//void WiFi_Config( void );
//void NVIC_Configuration( void );

//extern bool IS_WIFI_LOG_DEBUG;
//extern char WIFI_SSID_NAME[];
//extern char WIFI_PASSWORD[];
//extern char TCP_SERVER_IPADDRESS[];
//extern char TCP_SERVER_PORT[];
//extern char TCP_CILENT_IPADDRESS[];
//extern char TCP_CILENT_PORT[];


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
