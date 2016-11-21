/**
****************************************************************

File	:	cWIFI.c
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	code file

*****************************************************************
*/
#include <string.h>
#include <stdio.h>
#include "cWIFI.h"
#include "cmsis_os.h"

char WIFI_SSID_NAME[] = "YD-Xiaoxin";
char WIFI_PASSWORD[] = "12341234";
//char WIFI_SSID_NAME[] = "QK365";
//char WIFI_PASSWORD[] = "kunlong123456";

char TCP_SERVER_IPADDRESS[] = "alexyan.xyz";
char TCP_SERVER_PORT[] = "4000";
//char TCP_SERVER_IPADDRESS[] = "183.230.40.33";
//char TCP_SERVER_PORT[] = "80";
	 
char TCP_CILENT_IPADDRESS[] = "ESP_113AFD";
char TCP_CILENT_PORT[] = "8000";

HttpPacket httpPacket;
WifiState  wifiState;
WIFI_DataType wifiData;


//httpPacket.url = "/sub/api/vote/vote HTTP/1.1";
//httpPacket.host="cdi.tongji.edu.cn";
//httpPacket.contentType = "application/json";

//#ifdef __GNUC__  
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)  
//#else  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)  
//#endif /* __GNUC__ */  
//PUTCHAR_PROTOTYPE  
//{  
//    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 100);  
//    return ch;  
//}

/**
****************************************************************

Function：	cWIFI_Init
Author	:	@hiyangdong
Version	:	V1.0
date	:	2015-05-13
brief	:	initialize the RGB pins

*****************************************************************
*/

void cWIFI_Init(void)
{
//	MX_USART1_UART_Init();	//必须注释才能通过串口发送指令
	HAL_GPIO_WritePin(WIFI_WorkMode_GPIO_Port,WIFI_WorkMode_Pin,GPIO_PIN_RESET);	//下拉：工作模式;
	HAL_GPIO_WritePin(WIFI_PD_GPIO_Port,WIFI_PD_Pin,GPIO_PIN_SET);	//1）高电平工作;2）低电平模块供电关掉;
	HAL_Delay(1000);
//	cWIFI_Net_Mode_Choose(STA_AP);
////	cWIFI_Rst();
//	cWIFI_CIPMUX();
//	HAL_Delay(1000);
	
}

void cWIFI_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = UART_TX_Pin|UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_usart1_rx);

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
		HAL_DMA_Init(&hdma_usart1_tx);

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void cWIFI_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, UART_TX_Pin|UART_RX_Pin);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
} 

/*
 * 函数名：cWIFI_PD
 * 描述  ：使能/禁用WF-ESP8266模块
 * 输入  ：enumChoose = ON，使能模块
 *         enumChoose = OFF，禁用模块
 * 返回  : 无
 * 调用  ：被外部调用
 */
void cWIFI_PD ( WIFI_powerMode mode )
{
	if ( mode == ONx )
			HAL_GPIO_WritePin(WIFI_PD_GPIO_Port,WIFI_PD_Pin,GPIO_PIN_SET);	//1）高电平工作;2）低电平模块供电关掉;

	else
			HAL_GPIO_WritePin(WIFI_PD_GPIO_Port,WIFI_PD_Pin,GPIO_PIN_RESET);		
}


HAL_StatusTypeDef cWIFI_strWrite(char *pString)
{
//  uint16_t length = 0;
	HAL_StatusTypeDef status = HAL_OK;
	
//	length = strlen(pString)+2;
//	strcat(pString,"\r\n");
	status = HAL_UART_Transmit(&WIFI_UART, (uint8_t*)pString, strlen(pString), TX_TIMEOUT);
//	status = HAL_UART_Transmit(&WIFI_UART, "\r\n", 2, 10);

  return status;
}

HAL_StatusTypeDef cWIFI_strRead(char *pString, uint32_t timeout)
{

	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_UART_Receive(&WIFI_UART, (uint8_t*)pString, BUFFER_MAX, timeout);
	printf("%s",wifiData.buffer);
	
  return status;
}
/*
 * 函数名：cWIFI_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool cWIFI_Cmd (char * cmd, char * reply1, char * reply2, uint32_t timeout)
{    

	cWIFI_strWrite(cmd);
	osDelay(1);
	if((reply1 == 0) &&(reply2 == 0))                      //不需要接收数据
		return true;
	
	memset(wifiData.buffer, 0x00, BUFFER_MAX);
	cWIFI_strRead(wifiData.buffer,timeout);                //延时
	  
	if((reply1 != 0) &&(reply2 != 0))
	{
		return((bool) strstr(wifiData.buffer, reply1) ||(bool) strstr(wifiData.buffer, reply2)); 
	}
	else if(reply1 != 0)
	{
		return((bool) strstr(wifiData.buffer, reply1));
	}
	else
	{
		return((bool) strstr(wifiData.buffer, reply2));
	}
}

void cWIFI_Rst ( void )
{
//cWIFI_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	HAL_GPIO_WritePin(WIFI_PD_GPIO_Port,WIFI_PD_Pin,GPIO_PIN_RESET);
	osDelay(100);
	HAL_GPIO_WritePin(WIFI_PD_GPIO_Port,WIFI_PD_Pin,GPIO_PIN_SET);
	osDelay(1000); 
	wifiState.state = 1;
}

void cWIFI_AT_Test ( void )
{
	wifiState.error = 0;
	if(wifiState.state > 1)
	{
		while(cWIFI_Cmd ("AT", "OK", NULL, 250)==0)
		{
			wifiState.error++;
			if(wifiState.error > ERROR_MAX)
			{
				cWIFI_Rst();
				break;
			}
		}
	}
	else
	{
		cWIFI_Rst();
	}	
}

void cWIFI_ATE ( void )
{
	wifiState.error = 0;
	while(cWIFI_Cmd("ATE0","OK",NULL,200)==0)		//ATE0:关闭回显功能;ATE1:打开回显功能
	{
		wifiState.error++;
		if(wifiState.error > ERROR_MAX)
		{
			cWIFI_AT_Test();
			break;
		}
	}
	wifiState.state = 2;
}

/*
 * 函数名：cWIFI_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bool cWIFI_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
    switch ( enumMode )
    {
        case STA:
            return cWIFI_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 

        case AP:
            return cWIFI_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 

        case STA_AP:
            return cWIFI_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 

        default:
            return false;
    }
	
}

void cWIFI_CIPMUX ( void )
{
	wifiState.error = 0;
	while(cWIFI_Cmd("AT+CIPMODE=0","OK",NULL,200)==0)		//不透明传输:AT+CIPMODE=0
	{
		wifiState.error++;
		if(wifiState.error > ERROR_MAX)
		{
			cWIFI_AT_Test();
			wifiState.state = 1;
			break;
		}
	}
	wifiState.state = 2;
}

void cWIFI_CWLAP ( void )
{
	wifiState.error = 0;
	while(cWIFI_Cmd("AT+CWLAP","OK",NULL,200)==0)
	{
		wifiState.error++;
		if(wifiState.error > ERROR_MAX)
		{
			cWIFI_AT_Test();
			wifiState.state = 1;
			break;
		}
	}
	wifiState.state = 2;
}


/*
 * 函数名：cWIFI_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool cWIFI_CWJAP ( char * pSSID, char * pPassWord )
{
	char cCmd [128];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", pSSID, pPassWord );
	
	return cWIFI_Cmd ( cCmd, "OK", NULL, 7800 );
}

void cWIFI_JoinAP ( void )
{
	wifiState.error = 0;
	while(cWIFI_CWJAP(WIFI_SSID_NAME,WIFI_PASSWORD)==0)
	{
			wifiState.error++;
			if(wifiState.error > ERROR_MAX)
			{
				cWIFI_AT_Test();
				break;
			}
	}
}

/*
 * 函数名：cWIFI_STAJoinTCP
 * 描述  ：WF-ESP8266模块连接WiFi后连接tcp server
 * 输入  ：ip，tcp server地址
 *       ：port，tcp server 端口
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool cWIFI_STAJoinTCP ( char * ip, char * port )
{
	char cCmd [64];

	sprintf ( cCmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", ip, port );
	
	return cWIFI_Cmd ( cCmd, "ALREADY CONNECT", NULL, 3000 );		//2016-07-20 01:12:55
//	return cWIFI_Cmd ( cCmd, "OK", "ALREADY CONNECT", 700 );
}

void cWIFI_JoinTCP ( void )
{
	wifiState.error = 0;
	while(cWIFI_STAJoinTCP(TCP_SERVER_IPADDRESS,TCP_SERVER_PORT)==0)
	{
			wifiState.error++;
			if(wifiState.error > ERROR_MAX)
			{
				cWIFI_AT_Test();
				break;
			}
	}
}

void cWIFI_CIPCLOSE ( void )
{
	wifiState.error = 0;
	while(cWIFI_Cmd("AT+CIPCLOSE","CLOSE","OK",500)==0)
	{
		wifiState.error++;
		if(wifiState.error > ERROR_MAX)
		{
			cWIFI_AT_Test();
			wifiState.state = 1;
			break;
		}
	}
	wifiState.state = 2;
}


void cWIFI_CIPSEND ( void )
{
	char cCmd [20];
	wifiState.error = 0;
	
	sprintf ( cCmd, "AT+CIPSEND=%d\r\n", httpPacket.length);
	while(cWIFI_Cmd ( cCmd, "> ", 0, 3000 )==0)
	{
			wifiState.error++;
			if(wifiState.error > ERROR_MAX)
			{
				cWIFI_AT_Test();
				break;
			}
	}
}


bool cWIFI_StrSend (char * pStr)
{
	wifiState.error = 0;

	while(cWIFI_Cmd( pStr, "SEND OK", "success", 1000 )==0)
	{
			wifiState.error++;
			if(wifiState.error > ERROR_MAX)
			{
				cWIFI_AT_Test();
				break;
			}
	}

	return (bool) wifiState.error;

}


void cWIFI_State ( uint8_t state )
{
	switch(state)
	{
		case 0:		
						cWIFI_Rst();
						state = 1;
			break;
		case 1:	cWIFI_Cmd ("AT", "OK", NULL, 250);
	}
	while(!cWIFI_Cmd ("AT", "OK", NULL, 250))
  {
     cWIFI_Rst();
  }
}




bool cWIFI_SendString (char * pStr)
{
	char cCmd [20];
	bool bRet = false;
	uint16_t ulStrLength;
	
	ulStrLength = strlen ( pStr );	

	sprintf ( cCmd, "AT+CIPSEND=%d", ulStrLength);
		
	cWIFI_Cmd ( cCmd, "> ", 0, 2000 );

	bRet = cWIFI_Cmd ( pStr, "SEND OK", 0, 1000 );

	
	return bRet;

}

void cWIFI_Packet(OPS_TYPE operType, char* jsonData)
{
    int jsonLen = strlen(jsonData);
    char buf[64];

    memset(httpPacket.content, 0, CMDLEN);
    memset(buf, 0, sizeof(buf));

    switch(operType)
    {
        case POST:
						sprintf(buf, "POST %s\r\n", httpPacket.url);
						strcat(httpPacket.content, buf);
						
            if(strlen(httpPacket.content))
            {
                sprintf(buf, "Host: %s\r\n\r\n", httpPacket.host);
                strcat(httpPacket.content, buf);
								//GPRS
//								sprintf(buf, "Content-Type: %s\r\n", httpPacket.contentType);
//								strcat(httpPacket.content, buf);
								//GPRS end
							
								//WIFI
								sprintf(buf, "api-key: %s\r\n", httpPacket.apiKey);
								strcat(httpPacket.content, buf);
								//WIFI end
							
                sprintf(buf, "Content-Length: %d\r\n\r\n", jsonLen);
								strcat(httpPacket.content, buf);
								strcat(httpPacket.content, jsonData);
								httpPacket.length = strlen(httpPacket.content);
            }
            break;

        case GET:
            sprintf(buf, "GET %s\r\n", httpPacket.url);
						strcat(httpPacket.content, buf);
            sprintf(buf, "Host: %s\r\n\r\n", httpPacket.host);
            strcat(httpPacket.content, buf);
						httpPacket.length = strlen(httpPacket.content);
            break;

        default:
            break;
    }

//    if(strlen(content))
//    {
//        return RET_OK;
//    }
//    else
//    {
//        return RET_ERR;
//    }
}



