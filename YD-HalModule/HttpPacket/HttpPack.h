/**********************************************************************
 *  Copyright(c) 2014-2015 China Mobile M2M Company Limited.
 *  All rights reserved.
 *
 *  文件名称: HttpPacket.h
 *  简要描述: Arduino http packets encapsulation library
 *
 *  创建日期: 2015.4.24
 *  作者: Zhou Jiaxu
 *  说明:  Arduino http packets encapsulation library
 *  版本:  V2.1
 *
 *  修改日期:2015.5.29
 *  作者:Zhou Jiaxu
 *  说明:相对于V1.0修改关于批量查询数据流的函数，添加
 *               SDK使用示例
 *
 *  修改日期:2015.10.30
 *  作者:Zhou Jiaxu
 *  说明:相对于V2.0修改
 *              int createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, JsonObject&  jsonData);
 *                                        为 
 *              int createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, char* jsonData);
 *              V2.0使用JSON库, 导致内存占用过大，可能导致程序
 *              崩溃，V2.1直接使用封装好的JSON串作为输入，节省
 *              内存开销，降低耦合。
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
