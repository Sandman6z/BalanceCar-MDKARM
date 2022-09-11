/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	systick.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/

#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"


extern unsigned short SoftTimer[5];

void SoftTimerCountDown(void);
void SysTick_Init(void);


#endif /* __SYSTICK_H */

