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

//全部函数测试成功

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

#define mp3_Vol(a)		mp3_setting(0x31,a);		//设置音量:0~30 level
#define mp3_EQ(a)			mp3_setting(0x32,a);		//设置EQ:0-5(NO\POP\ROCK\JAZZ\CLASSIC\BASS)
#define mp3_Cycle(a)	mp3_setting(0x33,a);		//设置循环模式:0-4(全盘/文件夹/单曲/随机/无循环)
#define mp3_File(a)		mp3_setting(0x34,a);		//文件夹切换:0（上一文件夹），1(下一文件夹)
#define mp3_Device(a)	mp3_setting(0x35,a);		//设备切换:0（U 盘），1（SD）

#define mp3_Song(a)					mp3_select(0x41,(a>>8),(a&0xff));		//选择播放曲目:曲目为1-65536
#define mp3_FileSong(a,b)		mp3_select(0x42,a,b);								//指定文件夹曲目播放:文件夹必须命名为00-99，歌曲必须命名为001 XXX.MP3
#define mp3_InsertSong(a)		mp3_select(0x43,(a>>8),(a&0xff));		//插播功能:当播放完后，接着播放原来暂停的曲目。
#define mp3_InsertFileSong(a,b)		mp3_select(0x44,a,b);					//插播指定文件夹里的歌曲:当播放完后，接着播放原来暂停的曲目。


HAL_StatusTypeDef mp3_commamd(uint8_t cmd);
HAL_StatusTypeDef mp3_setting(uint8_t cmd, uint8_t para);
HAL_StatusTypeDef mp3_select(uint8_t cmd, uint8_t paraH, uint8_t paraL);

#endif
