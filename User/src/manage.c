/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	manage.c
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	


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


