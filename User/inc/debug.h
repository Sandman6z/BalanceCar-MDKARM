/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	debug.h
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 	




***********************************************************************/
#ifndef _DEBUG_H
#define _DEBUG_H

#include "usart.h"

#define DEBUG_EN				1


#define IMU_SCOPE_EN						1	// ʹ�����ģ��ʾ����������Ϣ
#define IMU_FOURAXISMONITOR_EN	0	// ʹ���������ģ����������Ϣ

#define COMMUNICATE_DEBUG_EN		0	// ʹ���������ͨ���ŵ�����Ϣ
#define INFRARE_DEBUG_EN				0	// KKDD mody 1->0 ʹ���������Ѱ��������Ϣ


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
