#include "sim900a.h"


char WIFI_SSID_NAME[] = "623network5_8GHz";
char WIFI_PASSWORD[] = "6958949599";//�˴���ʹ����������ip
char TCP_SERVER_IPADDRESS[] = "cdi.tongji.edu.cn";
char TCP_SERVER_PORT[] = "80";
char TCP_CILENT_IPADDRESS[] = "ESP_113AFD";
char TCP_CILENT_PORT[] = "8000";


/*
 * ��������SIM900A_Choose
 * ����  ��ʹ��/����WF-ESP8266ģ��
 * ����  ��enumChoose = ENABLE��ʹ��ģ��
 *         enumChoose = DISABLE������ģ��
 * ����  : ��
 * ����  �����ⲿ����
 */
void SIM900A_Choose ( FunctionalState enumChoose )
{
	if ( enumChoose == ENABLE )
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
	
	else
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);
	
}


/*
 * ��������SIM900A_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ����SIM900A_AT_Test����
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
 * ��������SIM900A_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
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


//��һ����AT+CSQ ��ѯ�����ź�����
//���е�һ������Ϊ�����ź��������Ϊ31������ֵԽ��˵�������ź�Խǿ
void SIM900A_CSQ ( void )
{
	SIM900A_Cmd ("AT+CSQ", "+CSQ", 0, 100)
}


//�ڶ�����AT+CREG? ��ѯ����ע�����
//���еڶ�������Ϊ1��5��˵���Ѿ�ע��ɹ���
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
 * ��������SIM900A_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool SIM900A_Cmd (char * cmd, char * reply1, char * reply2, uint32_t timeout)
{    

	Serial_strWrite(cmd);

	if((reply1 == 0) &&(reply2 == 0))                      //����Ҫ��������
		return true;
	
	Serial_strRead(SIM900A_Frame.RX_BUF,timeout);                //��ʱ
	  
	if((reply1 != 0) &&(reply2 != 0))
		return((bool) strstr(SIM900A_Frame.RX_BUF, reply1) ||(bool) strstr(SIM900A_Frame.RX_BUF, reply2)); 
 	
	else if(reply1 != 0)
		return((bool) strstr(SIM900A_Frame.RX_BUF, reply1));
	
	else
		return((bool) strstr(SIM900A_Frame.RX_BUF, reply2));
	

}


/*
 * ��������SIM900A_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
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
 * ��������SIM900A_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool SIM900A_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return SIM900A_Cmd ( cCmd, "OK", NULL, 7800 );
	
}

/*
 * ��������SIM900A_STAJoinTCP
 * ����  ��WF-ESP8266ģ������WiFi������tcp server
 * ����  ��ip��tcp server��ַ
 *       ��port��tcp server �˿�
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool SIM900A_STAJoinTCP ( char * ip, char * port )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CIPSTART=\"TCP\",\"%s\",%s", ip, port );
	
	return SIM900A_Cmd ( cCmd, "OK", "ALREAY CONNECT", 780 );
	
}

/*
 * ��������SIM900A_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool SIM900A_BuildAP ( char * pSSID, char * pPassWord, char * enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );
	
	return SIM900A_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * ��������SIM900A_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool SIM900A_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return SIM900A_Cmd ( cStr, "OK", 0, 500 );
	
}


/*
 * ��������SIM900A_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
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
//���� tcp�ĺ���
void SIM900A_linkTCP_join ( void )
{
	while(!SIM900A_STAJoinTCP(TCP_SERVER_IPADDRESS,TCP_SERVER_PORT));
}

/*
 * ��������SIM900A_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
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
 * ��������SIM900A_TransparentTransmission
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool SIM900A_TransparentTransmission ( void )
{
	return (SIM900A_Cmd ( "AT+CIPMODE=1", "OK", "Link is builded", 500 ) && SIM900A_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 ));
}


/*
 * ��������SIM900A_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����

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
 * ��������SIM900A_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
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
 * ��������SIM900A_STA_TCP_Client
 * ����  ��WF-ESP8266ģ�����STA TCP Clien����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
//void SIM900A_STA_TCP_Client ( void )
//{
//	IS_WIFI_LOG_DEBUG && PC_Usart("\r\nESP8266 WiFiģ�����\r\n");     //��ӡ����������ʾ��Ϣ
//	SIM900A_AT_Test ();
//	SIM900A_Net_Mode_Choose ( STA );

//	SIM900A_WIFIAP_join();
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Linked Wifi\r\n");

//	SIM900A_Enable_MultipleId ( DISABLE );
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Config module into TCP Client\r\n");
//    
//	SIM900A_linkTCP_join();//����  tcp �������ӵĺ���
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Connected TCP Server\r\n");
//    
//    // �رմ���2�����ж� ʹ�ܴ���2�����ж� �����������  ����͸��ģʽ
//    while(!SIM900A_TransparentTransmission());
//    USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    SetUART2_NVIC_ISENABLE(1);
//    USART2ReceiveHandler = ReceiveUSART2PacketDelegate;
//    
//    IS_WIFI_LOG_DEBUG && PC_Usart ( "Change into Transparent Transmission\r\n");
//}


///*
// * ��������SIM900A_AP_TCP_Server
// * ����  ��WF-ESP8266ģ�����AP TCP Server����
// * ����  ����
// * ����  : ��
// * ����  �����ⲿ����
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


//	PC_Usart ( "\r\n������Ҫ������WiFi�����ơ����ܷ�ʽ����Կ�����ܷ�ʽ�ı��Ϊ��\
//              \r\n0 = OPEN\
//              \r\n1 = WEP\
//              \r\n2 = WPA_PSK\
//	            \r\n3 = WPA2_PSK\
//              \r\n4 = WPA_WPA2_PSK\
//							\r\n�����ʽΪ�������ַ�+Ӣ�Ķ���+���ܷ�ʽ���+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

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
//	PC_Usart ( "\r\n�����������Ҫ�����Ķ˿ںźͳ�ʱʱ�䣨0~28800����λ���룩�������ʽΪ���˿ں��ַ�+Ӣ�Ķ���+��ʱʱ���ַ�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

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
//		PC_Usart ( "\r\n����ѯ��ģ��IP����\r\n" );
//	  SIM900A_Cmd ( "AT+CIFSR", "OK", "Link", 500 );
//		
//		PC_Usart ( "\r\n�����ֻ����ӸղŴ�����WiFi������ֻ����һ���ֻ�����ΪID0��Ȼ�����ֻ��������������TCP Client���Ӹղſ����ķ�������AP IP������\r\n" );
//		Delay_ms ( 20000 ) ;
//	}	while ( ! SIM900A_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:0", 0, 500 ) );
//	

//	PC_Usart ( "\r\n������Ҫ��˿��ֻ���ID0�����͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );

//	ul = strlen ( cStrInput );
//	
//	SIM900A_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );

//	
//	PC_Usart ( "\r\n�����ֻ�����������ַ����ַ���\r\n" );
//	while (1)
//	{
//	  pStr = SIM900A_ReceiveString ( DISABLE );
//		PC_Usart ( "%s", pStr );
//	}

//}


///*
// * ��������SIM900A_StaTcpClient_ApTcpServer
// * ����  ��WF-ESP8266ģ�����STA(TCP Client)+AP(TCP Server)����
// * ����  ����
// * ����  : ��
// * ����  �����ⲿ����
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


//	PC_Usart ( "\r\n������Ҫ������WiFi�����ơ����ܷ�ʽ����Կ�����ܷ�ʽ�ı��Ϊ��\
//						\r\n0 = OPEN\
//						\r\n1  =WEP\
//						\r\n2 = WPA_PSK\
//						\r\n3 = WPA2_PSK\
//						\r\n4 = WPA_WPA2_PSK\
//						\r\n�����ʽΪ�������ַ�+Ӣ�Ķ���+���ܷ�ʽ���+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

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
//		PC_Usart ( "\r\n������Ҫ���ӵ�WiFi���ƺ���Կ�������ʽΪ�������ַ�+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

//		scanf ( "%s", cStrInput );

//		PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

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
//	PC_Usart ( "\r\n�����������Ҫ�����Ķ˿ںźͳ�ʱʱ�䣨0~28800����λ���룩�������ʽΪ���˿ں��ַ�+Ӣ�Ķ���+��ʱʱ���ַ�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );

//	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

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
//		PC_Usart ( "\r\n���ڵ����Ͻ��������������TCP Server�������磬��������Ե�IP�Ͷ˿ںţ������ʽΪ������IP+Ӣ�Ķ���+�˿ں�+�ո񣬵������\r\n" );

//		scanf ( "%s", cStrInput );

//		PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

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
//		PC_Usart ( "\r\n����ѯ��ģ��IP��ǰһ��ΪAP IP����һ��ΪSTA IP����\r\n" );
//	  SIM900A_Cmd ( "AT+CIFSR", "OK", "Link", 500 );
//		
//		PC_Usart ( "\r\n�����ֻ����ӸղŴ�����WiFi������ֻ����һ���ֻ�����ΪID3��Ȼ�����ֻ��������������TCP Client���Ӹղſ����ķ�������AP IP������\r\n" );
//		Delay_ms ( 20000 ) ;
//	}	while ( ! SIM900A_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:3", 0, 500 ) );
//	

//	for ( uc = 0; uc < 3; uc ++ )
//	{
//		PC_Usart ( "\r\n������˿�ID%dҪ���͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n", uc );

//		scanf ( "%s", cStrInput );

//		ul = strlen ( cStrInput );
//		
//		SIM900A_SendString ( DISABLE, cStrInput, ul, ( ENUM_ID_NO_TypeDef ) uc );
//		
//	}
//	
//	
//	PC_Usart ( "\r\n������Ҫ��˿��ֻ���ID3�����͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );

//	ul = strlen ( cStrInput );
//	
//	SIM900A_SendString ( DISABLE, cStrInput, ul, Multiple_ID_3 );

//	
//	PC_Usart ( "\r\n���ڵ�������������ֻ��ֻ�����������ַ����ַ�������\r\n" );
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

