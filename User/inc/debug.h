/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	debug.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/
#ifndef _DEBUG_H
#define _DEBUG_H

#include "usart.h"

#define DEBUG_EN				1


#define IMU_SCOPE_EN						1	// 使能输出模拟示波器调试信息
#define IMU_FOURAXISMONITOR_EN	0	// 使能输出四轴模拟器调试信息

#define COMMUNICATE_DEBUG_EN		0	// 使能输出蓝牙通信信调试信息
#define INFRARE_DEBUG_EN				0	// KKDD mody 1->0 使能输出红外寻迹调试信息


#if DEBUG_EN
#define DebugOutByte(byte)	Uart1SendByte(byte)
#define DebugOutBuff(buff, len)	Uart1SendBuff(buff, len)
#define DebugOutStr(str)		Uart1SendStr(str)
#else
#define DebugOutByte(byte)
#define DebugOutBuff(buff,len)
#define DebugOutStr(str)
#endif


void DebugService(void);




#endif 
