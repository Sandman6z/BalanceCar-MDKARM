/**********************************************************************
��Ȩ���У�	���ش��¿Ƽ���2017.
��		����	http://www.miaowlabs.com
��		����	https://miaowlabs.taobao.com/
�� �� ��: 	manage.c
��    ��:   ����ʵ����
��		��:   3.00
�������:   2017.03.01
��		Ҫ: 	


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


