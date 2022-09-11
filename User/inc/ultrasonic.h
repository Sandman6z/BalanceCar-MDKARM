/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	ultrasonic.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/

#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

extern int Distance;


void TIM1_Cap_Init(void);	
void Read_Distane(void);
char InfraredDetect(void);
void UltraSelfCheck(void);
int IsUltraOK(void);



#endif

