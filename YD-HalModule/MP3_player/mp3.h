/*****************************************************************

File			:	YD-Module\MP3\MP3.h
Fuction		:	
Author		:	@hiyangdong
Version		:	V1.0
Time			:	2016-05-28 16:35:58

*****************************************************************/

#ifndef _MP3_H_
#define _MP3_H_


#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart2;

#define MP3_UART   huart2
#define MP3_VOLUME 3

//ȫ���������Գɹ�

#define mp3_Play			mp3_commamd(0x01);
#define mp3_Pause			mp3_commamd(0x02);
#define mp3_Next			mp3_commamd(0x03);
#define mp3_Prev			mp3_commamd(0x04);
#define mp3_VolAdd		mp3_commamd(0x05);
#define mp3_VolLow		mp3_commamd(0x06);
#define mp3_Sleep			mp3_commamd(0x07);
#define mp3_Reset			mp3_commamd(0x09);
#define mp3_Speed			mp3_commamd(0x0A);
#define mp3_Slow			mp3_commamd(0x0B);
#define mp3_Stop			mp3_commamd(0x0D);

#define mp3_Vol(a)		mp3_setting(0x31,a);		//��������:0~30 level
#define mp3_EQ(a)			mp3_setting(0x32,a);		//����EQ:0-5(NO\POP\ROCK\JAZZ\CLASSIC\BASS)
#define mp3_Cycle(a)	mp3_setting(0x33,a);		//����ѭ��ģʽ:0-4(ȫ��/�ļ���/����/���/��ѭ��)
#define mp3_File(a)		mp3_setting(0x34,a);		//�ļ����л�:0����һ�ļ��У���1(��һ�ļ���)
#define mp3_Device(a)	mp3_setting(0x35,a);		//�豸�л�:0��U �̣���1��SD��

#define mp3_Song(a)					mp3_select(0x41,(a>>8),(a&0xff));		//ѡ�񲥷���Ŀ:��ĿΪ1-65536
#define mp3_FileSong(a,b)		mp3_select(0x42,a,b);								//ָ���ļ�����Ŀ����:�ļ��б�������Ϊ00-99��������������Ϊ001 XXX.MP3
#define mp3_InsertSong(a)		mp3_select(0x43,(a>>8),(a&0xff));		//�岥����:��������󣬽��Ų���ԭ����ͣ����Ŀ��
#define mp3_InsertFileSong(a,b)		mp3_select(0x44,a,b);					//�岥ָ���ļ�����ĸ���:��������󣬽��Ų���ԭ����ͣ����Ŀ��


HAL_StatusTypeDef mp3_commamd(uint8_t cmd);
HAL_StatusTypeDef mp3_setting(uint8_t cmd, uint8_t para);
HAL_StatusTypeDef mp3_select(uint8_t cmd, uint8_t paraH, uint8_t paraL);

#endif
