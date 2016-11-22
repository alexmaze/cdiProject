/**
  ******************************************************************************
  * @file    wifi_config.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   WiFiģ��ӿ���������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/

#include "wifi_config.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"


struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
struct  STRUCT_USART1_1_Fram str1_1esp8266 = {0} ;

bool IS_WIFI_LOG_DEBUG = true;

char WIFI_SSID_NAME[] = "Signal_Process_PandoraBox2.4";
char WIFI_PASSWORD[] = "zhanglaoshidianhuahaoma";//�˴���ʹ����������ip
char TCP_SERVER_IPADDRESS[] = "TIAN";
char TCP_SERVER_PORT[] = "31500";
char TCP_CILENT_IPADDRESS[] = "ESP_113AFD";
char TCP_CILENT_PORT[] = "50000";


/**
  * @brief  WiFi_Config wifi ��ʼ��
  * @param  ��
  * @retval ��
  */
void WiFi_Config( void )
{
	WiFi_RST_INIT();
    WiFi_led_INIT();
 	WiFi_USART1_INIT(); 
	WiFi_USART2_INIT(); 
	WiFi_NVIC_INIT();
}


/// ����NVIC�е�UART2�ж�
void NVIC_Configuration( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


   /////////////////////////////////
   /*
   ���� �����ж�  1 �ĳ�ʼ������
   
   */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


/*********************************************************end of file**************************************************/
