/**********************************************************************
 *  Copyright(c) 2014-2015 China Mobile M2M Company Limited.
 *  All rights reserved.
 *
 *  �ļ�����: HttpPacket.h
 *  ��Ҫ����: Arduino http packets encapsulation library
 *
 *  ��������: 2015.4.24
 *  ����: Zhou Jiaxu
 *  ˵��:  Arduino http packets encapsulation library
 *  �汾:  V2.1
 *
 *  �޸�����:2015.5.29
 *  ����:Zhou Jiaxu
 *  ˵��:�����V1.0�޸Ĺ���������ѯ�������ĺ��������
 *               SDKʹ��ʾ��
 *
 *  �޸�����:2015.10.30
 *  ����:Zhou Jiaxu
 *  ˵��:�����V2.0�޸�
 *              int createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, JsonObject&  jsonData);
 *                                        Ϊ 
 *              int createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, char* jsonData);
 *              V2.0ʹ��JSON��, �����ڴ�ռ�ù��󣬿��ܵ��³���
 *              ������V2.1ֱ��ʹ�÷�װ�õ�JSON����Ϊ���룬��ʡ
 *              �ڴ濪����������ϡ�
 **********************************************************************/

#ifndef _HTTP_PACKET_H_
#define _HTTP_PACKET_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>


#define CMDLEN 300

typedef enum operation_type
{
    POST,
    PUT,
    GET,
    DELETE
}OPS_TYPE;

typedef struct http_packet
{
	char* url;
	char* host;
	char* contentType;
	char* contentLength;
	char* apiKey;
	char  content[CMDLEN];
	uint16_t length;
	
}HttpPacket;




#endif  //HTTP_PACKET_H_
