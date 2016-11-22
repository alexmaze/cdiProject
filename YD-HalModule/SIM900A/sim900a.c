#include "sim900a.h"


char WIFI_SSID_NAME[] = "623network5_8GHz";
char WIFI_PASSWORD[] = "6958949599";//此处可使用主机名和ip
char TCP_SERVER_IPADDRESS[] = "cdi.tongji.edu.cn";
char TCP_SERVER_PORT[] = "80";
char TCP_CILENT_IPADDRESS[] = "ESP_113AFD";
char TCP_CILENT_PORT[] = "8000";


/*
 * 函数名：SIM900A_Choose
 * 描述  ：使能/禁用WF-ESP8266模块
 * 输入  ：enumChoose = ENABLE，使能模块
 *         enumChoose = DISABLE，禁用模块
 * 返回  : 无
 * 调用  ：被外部调用
 */
void SIM900A_Choose ( FunctionalState enumChoose )
{
	if ( enumChoose == ENABLE )
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
	
	else
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);
	
}


/*
 * 函数名：SIM900A_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被SIM900A_AT_Test调用
 */
void SIM900A_Rst ( void )
{

//    SIM900A_Cmd ( "AT+RST", "OK", "ready", 2500 );   	

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_Delay(100); 
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_Delay(2000); 
}


/*
 * 函数名：SIM900A_AT_Test
 * 描述  ：对WF-ESP8266模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void SIM900A_AT_Test ( void )
{

	while (!SIM900A_Cmd ("AT", "OK", NULL, 100))
    {
        SIM900A_Rst();
    }

}


void SIM900A_GSV ( void )
{

	while (!SIM900A_Cmd ("AT+GSV", "OK", SIMCOM_Ltd, 1000))
    {
        SIM900A_Rst();
    }

}

void SIM900A_ATE ( uint8_t returndisplay )
{
	if(returndisplay==0)
	{
		SIM900A_Cmd ("ATE0", "OK", 0, 100)
	}
	else
	{
		SIM900A_Cmd ("ATE1", "OK", "ATE1", 500)
	}

}


//第一步：AT+CSQ 查询网络信号质量
//其中第一个参数为网络信号质量最大为31，此数值越大说明网络信号越强
void SIM900A_CSQ ( void )
{
	SIM900A_Cmd ("AT+CSQ", "+CSQ", 0, 100)
}


//第二步：AT+CREG? 查询网络注册情况
//其中第二个参数为1或5则说明已经注册成功。
void SIM900A_CREG ( void )
{
	SIM900A_Cmd ("AT+CREG?", "+CREG", "0,1", 100)
}

HAL_StatusTypeDef SIM900A_strWrite(char *pString)
{
  uint16_t length = 0;
	HAL_StatusTypeDef status = HAL_OK;
	
	length = strlen(pString);
	
	status = HAL_UART_Transmit(&WIFI_UART, pString, length, TX_TIMEOUT);
	status = HAL_UART_Transmit(&WIFI_UART, "\r\n", 2, 10);

  return status;
}

HAL_StatusTypeDef Serial_strRead(char *pString, uint32_t timeout)
{

	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_UART_Receive(&WIFI_UART, pString, len, timeout);
	HAL_UART_Transmit(&Debug_UART, pString, len, 100);
	
  return status;
}
/*
 * 函数名：SIM900A_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool SIM900A_Cmd (char * cmd, char * reply1, char * reply2, uint32_t timeout)
{    

	Serial_strWrite(cmd);

	if((reply1 == 0) &&(reply2 == 0))                      //不需要接收数据
		return true;
	
	Serial_strRead(SIM900A_Frame.RX_BUF,timeout);                //延时
	  
	if((reply1 != 0) &&(reply2 != 0))
		return((bool) strstr(SIM900A_Frame.RX_BUF, reply1) ||(bool) strstr(SIM900A_Frame.RX_BUF, reply2)); 
 	
	else if(reply1 != 0)
		return((bool) strstr(SIM900A_Frame.RX_BUF, reply1));
	
	else
		return((bool) strstr(SIM900A_Frame.RX_BUF, reply2));
	

}


/*
 * 函数名：SIM900A_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bool SIM900A_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
    switch ( enumMode )
    {
        case STA:
            return SIM900A_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 

        case AP:
            return SIM900A_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 

        case STA_AP:
            return SIM900A_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 

        default:
            return false;
    }
	
}


/*
 * 函数名：SIM900A_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool SIM900A_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return SIM900A_Cmd ( cCmd, "OK", NULL, 7800 );
	
}

/*
 * 函数名：SIM900A_STAJoinTCP
 * 描述  ：WF-ESP8266模块连接WiFi后连接tcp server
 * 输入  ：ip，tcp server地址
 *       ：port，tcp server 端口
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool SIM900A_STAJoinTCP ( char * ip, char * port )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CIPSTART=\"TCP\",\"%s\",%s", ip, port );
	
	return SIM900A_Cmd ( cCmd, "OK", "ALREAY CONNECT", 780 );
	
}

/*
 * 函数名：SIM900A_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
bool SIM900A_BuildAP ( char * pSSID, char * pPassWord, char * enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );
	
	return SIM900A_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * 函数名：SIM900A_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool SIM900A_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return SIM900A_Cmd ( cStr, "OK", 0, 500 );
	
}


/*
 * 函数名：SIM900A_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool SIM900A_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
    char cStr [100] = { 0 }, cCmd [120];

    switch (  enumE )
    {
        case enumTCP:
            sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
            break;

        case enumUDP:
            sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
            break;

        default:
            break;
    }

    if ( id < 5 )
        sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

    else
        sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

    return SIM900A_Cmd ( cCmd, "OK", "ALREAY CONNECT", 500 );
	
}
//连接 tcp的函数
void SIM900A_linkTCP_join ( void )
{
	while(!SIM900A_STAJoinTCP(TCP_SERVER_IPADDRESS,TCP_SERVER_PORT));
}

/*
 * 函数名：SIM900A_StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
 */
bool SIM900A_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( SIM900A_Cmd ( cCmd1, "OK", 0, 500 ) && SIM900A_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return SIM900A_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * 函数名：SIM900A_TransparentTransmission
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool SIM900A_TransparentTransmission ( void )
{
	return (SIM900A_Cmd ( "AT+CIPMODE=1", "OK", "Link is builded", 500 ) && SIM900A_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 ));
}


/*
 * 函数名：SIM900A_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用

 //	SIM900A_SendString ( DISABLE, cStrInput, ul, ( ENUM_ID_NO_TypeDef ) uc );
 */
bool SIM900A_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if ( enumEnUnvarnishTx )
		SIM900A_Usart ( "%s", pStr );

	
	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		SIM900A_Cmd ( cStr, "> ", 0, 1000 );

		bRet = SIM900A_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}
void SIM900A_WIFIAP_join ( void )
{
	while(!SIM900A_JoinAP(WIFI_SSID_NAME,WIFI_PASSWORD));	
}


/*
 * 函数名：SIM900A_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
 */
//char * SIM900A_ReceiveString ( FunctionalState enumEnUnvarnishTx )
//{
//	char * pRecStr = 0;
//	
//	strSIM900A_Fram_Record .InfBit .FramLength = 0;
//	strSIM900A_Fram_Record .InfBit .FramFinishFlag = 0;
//	while ( ! strSIM900A_Fram_Record .InfBit .FramFinishFlag );
//	strSIM900A_Fram_Record .Data_RX_BUF [ strSIM900A_Fram_Record .InfBit .FramLength ] = '\0';
//	
//	if ( enumEnUnvarnishTx )
//	{
//		if ( strstr ( strSIM900A_Fram_Record .Data_RX_BUF, ">" ) )
//			pRecStr = strSIM900A_Fram_Record .Data_RX_BUF;

//	}
//	
//	else 
//	{
//		if ( strstr ( strSIM900A_Fram_Record .Data_RX_BUF, "+IPD" ) )
//			pRecStr = strSIM900A_Fram_Record .Data_RX_BUF;

//	}

//	return pRecStr;
//}

//void SIM900A_TCP_Transparent_SendTest()
//{
//    PC_Usart ( "Send Date test\r\n");
//    
//    SIM900A_Usart("6345tsdf");
//    sendUart2OneByte(0x88);
//    sendUart2OneByte(0xEF);
//    sendUart2OneByte(0x00);
//    sendUart2OneByte(0xFF);
//    sendUart2OneByte(0x00);
//    sendUart2OneByte(0x2B);
//    sendUart2OneByte(0x2B);
//    sendUart2OneByte(0x2B);
//    sendUart2OneByte(0xCC);
//    SIM900A_Usart("fasdf+++fgh8468++a");
//    
//    
//}

/*
 * 函数名：SIM900A_STA_TCP_Client
 * 描述  ：WF-ESP8266模块进行STA TCP Clien测试
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
//void SIM900A_STA_TCP_Client ( void )
//{
//	IS_WIFI_LOG_DEBUG && PC_Usart("\r\nESP8266 WiFi模块测试\r\n");     //打印测试例程提示信息
//	SIM900A_AT_Test ();
//	SIM900A_Net_Mode_Choose ( STA );

//	SIM900A_WIFIAP_join();
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Linked Wifi\r\n");

//	SIM900A_Enable_MultipleId ( DISABLE );
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Config module into TCP Client\r\n");
//    
//	SIM900A_linkTCP_join();//处理  tcp 服务连接的函数
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Connected TCP Server\r\n");
//    
//    // 关闭串口2空闲中断 使能串口2接收中断 配置相关设置  进入透传模式
//    while(!SIM900A_TransparentTransmission());
//    USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    SetUART2_NVIC_ISENABLE(1);
//    USART2ReceiveHandler = ReceiveUSART2PacketDelegate;
//    
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Change into Transparent Transmission\r\n");
//}


///*
// * 函数名：SIM900A_AP_TCP_Server
// * 描述  ：WF-ESP8266模块进行AP TCP Server测试
// * 输入  ：无
// * 返回  : 无
// * 调用  ：被外部调用
// */
//void SIM900A_AP_TCP_Server ( void )
//{
//	char cStrInput [100] = { 0 }, * pStrDelimiter [3], * pBuf, * pStr;
//	u8 uc = 0;
// 	 u32 ul = 0;

//    SIM900A_Choose ( ENABLE );

//	SIM900A_AT_Test ();
//	
//	SIM900A_Net_Mode_Choose ( AP );


//	PC_Usart ( "\r\n请输入要创建的WiFi的名称、加密方式和密钥，加密方式的编号为：\
//              \r\n0 = OPEN\
//              \r\n1 = WEP\
//              \r\n2 = WPA_PSK\
//	            \r\n3 = WPA2_PSK\
//              \r\n4 = WPA_WPA2_PSK\
//							\r\n输入格式为：名称字符+英文逗号+加密方式编号+英文逗号+密钥字符+空格，点击发送\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n稍等片刻 ……\r\n" );

//	pBuf = cStrInput;
//	uc = 0;
//	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//	{
//		pStrDelimiter [ uc ++ ] = pStr;
//		pBuf = NULL;
//	} 
//	
//	SIM900A_BuildAP ( pStrDelimiter [0], pStrDelimiter [2], pStrDelimiter [1] );
//	SIM900A_Cmd ( "AT+RST", "OK", "ready", 2500 ); //*
//		

//	SIM900A_Enable_MultipleId ( ENABLE );
//		
//	
//	PC_Usart ( "\r\n请输入服务器要开启的端口号和超时时间（0~28800，单位：秒），输入格式为：端口号字符+英文逗号+超时时间字符+空格，点击发送\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n稍等片刻 ……\r\n" );

//	pBuf = cStrInput;
//	uc = 0;
//	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//	{
//		pStrDelimiter [ uc ++ ] = pStr;
//		pBuf = NULL;
//	} 

//	SIM900A_StartOrShutServer ( ENABLE, pStrDelimiter [0], pStrDelimiter [1] );
//	
//	
//	do
//	{
//		PC_Usart ( "\r\n正查询本模块IP……\r\n" );
//	  SIM900A_Cmd ( "AT+CIFSR", "OK", "Link", 500 );
//		
//		PC_Usart ( "\r\n请用手机连接刚才创建的WiFi，这里只连接一个手机，作为ID0，然后用手机网络调试助手以TCP Client连接刚才开启的服务器（AP IP）……\r\n" );
//		Delay_ms ( 20000 ) ;
//	}	while ( ! SIM900A_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:0", 0, 500 ) );
//	

//	PC_Usart ( "\r\n请输入要向端口手机（ID0）发送的字符串，输入格式为：字符串（不含空格）+空格，点击发送\r\n" );

//	scanf ( "%s", cStrInput );

//	ul = strlen ( cStrInput );
//	
//	SIM900A_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );

//	
//	PC_Usart ( "\r\n请在手机网络调试助手发送字符串\r\n" );
//	while (1)
//	{
//	  pStr = SIM900A_ReceiveString ( DISABLE );
//		PC_Usart ( "%s", pStr );
//	}

//}


///*
// * 函数名：SIM900A_StaTcpClient_ApTcpServer
// * 描述  ：WF-ESP8266模块进行STA(TCP Client)+AP(TCP Server)测试
// * 输入  ：无
// * 返回  : 无
// * 调用  ：被外部调用
// */
//void SIM900A_StaTcpClient_ApTcpServer ( void )
//{
//	char cStrInput [100] = { 0 }, * pStrDelimiter [3], * pBuf, * pStr;
//	u8 uc = 0;
//  u32 ul = 0;

//  SIM900A_Choose ( ENABLE );

//	SIM900A_AT_Test ();
//	
//	SIM900A_Net_Mode_Choose ( STA_AP );


//	PC_Usart ( "\r\n请输入要创建的WiFi的名称、加密方式和密钥，加密方式的编号为：\
//						\r\n0 = OPEN\
//						\r\n1  =WEP\
//						\r\n2 = WPA_PSK\
//						\r\n3 = WPA2_PSK\
//						\r\n4 = WPA_WPA2_PSK\
//						\r\n输入格式为：名称字符+英文逗号+加密方式编号+英文逗号+密钥字符+空格，点击发送\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n稍等片刻 ……\r\n" );

//	pBuf = cStrInput;
//	uc = 0;
//	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//	{
//		pStrDelimiter [ uc ++ ] = pStr;
//		pBuf = NULL;
//	} 
//	
//	SIM900A_BuildAP ( pStrDelimiter [0], pStrDelimiter [2], pStrDelimiter [1] );
//	SIM900A_Cmd ( "AT+RST", "OK", "ready", 2500 ); //*
//	

//	SIM900A_Cmd ( "AT+CWLAP", "OK", 0, 5000 );
//		
//  do
//	{
//		PC_Usart ( "\r\n请输入要连接的WiFi名称和密钥，输入格式为：名称字符+英文逗号+密钥字符+空格，点击发送\r\n" );

//		scanf ( "%s", cStrInput );

//		PC_Usart ( "\r\n稍等片刻 ……\r\n" );

//		pBuf = cStrInput;
//		uc = 0;
//		while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//		{
//			pStrDelimiter [ uc ++ ] = pStr;
//			pBuf = NULL;
//		} 
//		
//  } while ( ! SIM900A_JoinAP ( pStrDelimiter [0], pStrDelimiter [1] ) );

//	
//	SIM900A_Enable_MultipleId ( ENABLE );
//		
//	
//	PC_Usart ( "\r\n请输入服务器要开启的端口号和超时时间（0~28800，单位：秒），输入格式为：端口号字符+英文逗号+超时时间字符+空格，点击发送\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n稍等片刻 ……\r\n" );

//	pBuf = cStrInput;
//	uc = 0;
//	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//	{
//		pStrDelimiter [ uc ++ ] = pStr;
//		pBuf = NULL;
//	} 

//	SIM900A_StartOrShutServer ( ENABLE, pStrDelimiter [0], pStrDelimiter [1] );
//	
//	
//	do 
//	{
//		PC_Usart ( "\r\n请在电脑上将网络调试助手以TCP Server连接网络，并输入电脑的IP和端口号，输入格式为：电脑IP+英文逗号+端口号+空格，点击发送\r\n" );

//		scanf ( "%s", cStrInput );

//		PC_Usart ( "\r\n稍等片刻 ……\r\n" );

//		pBuf = cStrInput;
//		uc = 0;
//		while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//		{
//			pStrDelimiter [ uc ++ ] = pStr;
//			pBuf = NULL;
//		} 
//		
//  } while ( ! ( SIM900A_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_0 ) &&
//	              SIM900A_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_1 ) &&
//	              SIM900A_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_2 ) ) );
//		
//	
//	do
//	{
//		PC_Usart ( "\r\n正查询本模块IP，前一个为AP IP，后一个为STA IP……\r\n" );
//	  SIM900A_Cmd ( "AT+CIFSR", "OK", "Link", 500 );
//		
//		PC_Usart ( "\r\n请用手机连接刚才创建的WiFi，这里只连接一个手机，作为ID3，然后用手机网络调试助手以TCP Client连接刚才开启的服务器（AP IP）……\r\n" );
//		Delay_ms ( 20000 ) ;
//	}	while ( ! SIM900A_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:3", 0, 500 ) );
//	

//	for ( uc = 0; uc < 3; uc ++ )
//	{
//		PC_Usart ( "\r\n请输入端口ID%d要发送的字符串，输入格式为：字符串（不含空格）+空格，点击发送\r\n", uc );

//		scanf ( "%s", cStrInput );

//		ul = strlen ( cStrInput );
//		
//		SIM900A_SendString ( DISABLE, cStrInput, ul, ( ENUM_ID_NO_TypeDef ) uc );
//		
//	}
//	
//	
//	PC_Usart ( "\r\n请输入要向端口手机（ID3）发送的字符串，输入格式为：字符串（不含空格）+空格，点击发送\r\n" );

//	scanf ( "%s", cStrInput );

//	ul = strlen ( cStrInput );
//	
//	SIM900A_SendString ( DISABLE, cStrInput, ul, Multiple_ID_3 );

//	
//	PC_Usart ( "\r\n请在电脑网络调试助手或手机网络调试助手发送字符串……\r\n" );
//	while (1)
//	{
//	  pStr = SIM900A_ReceiveString ( DISABLE );
//		PC_Usart ( "%s", pStr );
//	}
//	
//}

// void Linktcp_LED_ON()
//{
//	Delay_ms(500);
//	GPIO_SetBits( GPIOB, GPIO_Pin_0 );
//	GPIO_SetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_3);	
//	Delay_ms(500);
//	GPIO_ResetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_3);
//	GPIO_ResetBits( GPIOB, GPIO_Pin_0 );	 
//}
//void LED_all_OFF()
//{
//    GPIO_SetBits( GPIOB, GPIO_Pin_0 );
//    GPIO_SetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_3);	
//}

