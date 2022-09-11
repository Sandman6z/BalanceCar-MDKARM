/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	dataflash.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/

#ifndef _DATAFLASH_H
#define _DATAFLASH_H


void ProgramFlash(unsigned int addr, char* data, unsigned short len);
void ReadFlash(unsigned int addr, char* data, unsigned short len);
void PIDInit(void);
void PIDWrite(char flag);
void PIDRead(void);
void PIDReset(char flag);


#endif

