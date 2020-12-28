/**********************************************************************
版权所有：	喵呜创新科技，2017.
官		网：	http://www.miaowlabs.com
淘		宝：	https://miaowlabs.taobao.com/
文 件 名: 	manage.c
作    者:   喵呜实验室
版		本:   3.00
完成日期:   2017.03.01
概		要: 	


***********************************************************************/

#include "manage.h"



const char FirmwareVer[] = "1+1>2";
const char EEPROMVer[]  = "1+1>2";
const char MCUVer[] = "F103C8T6";

//系统运行时间计数，，开机后开始计数，每秒增1
unsigned short  g_RunTime = 0;

//电池电压，实际值*100
unsigned short  g_BatVolt=0;

//小车运行模式:遥控模式、红外寻迹模式，超声波跟随模式
unsigned char g_CarRunningMode = CONTROL_MODE;


