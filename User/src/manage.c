/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	manage.c
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 	


***********************************************************************/

#include "manage.h"



const char FirmwareVer[] = "1+1>2";
const char EEPROMVer[]  = "1+1>2";
const char MCUVer[] = "F103C8T6";

//ϵͳ����ʱ���������������ʼ������ÿ����1
unsigned short  g_RunTime = 0;

//��ص�ѹ��ʵ��ֵ*100
unsigned short  g_BatVolt=0;

//С������ģʽ:ң��ģʽ������Ѱ��ģʽ������������ģʽ
unsigned char g_CarRunningMode = CONTROL_MODE;


