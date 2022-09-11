/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	communicate.h
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 	




***********************************************************************/
#ifndef _COMMUNICATE_H
#define _COMMUNICATE_H


typedef enum _PIDType{AnglePID, SpeedPID,}PIDType;


extern char StatusFlag;

void Parse(char *dataInput);
unsigned char ParseAT(char *dataInput);//KKDD
void ResponseIMU(void);
void ResponsePID(PIDType type);
void ResponseStatus(void);


#endif

