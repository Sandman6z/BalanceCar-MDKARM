/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	infrare.h
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 	




***********************************************************************/

#ifndef _INFRARE_H
#define _INFRARE_H


typedef enum _infrared_channel_t{
	infrared_channel_La = (0x01 << 0),
	infrared_channel_Lb = (0x01 << 1),
	infrared_channel_Lc = (0x01 << 2),
	infrared_channel_Ra = (0x01 << 4),
	infrared_channel_Rb = (0x01 << 5),
	infrared_channel_Rc = (0x01 << 6)
}infrared_channel_t;


void InfraredIOInit(void);
char InfraredDetect(void);
void InfrareSelfCheck(void);
int IsInfrareOK(void);

#endif

