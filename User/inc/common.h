/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	common.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	


***********************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

void delay_us(unsigned int n);
void delay_ms(unsigned int n);
void get_ms(unsigned long *time);


unsigned char XOR_Get(char * str, unsigned char  len);
unsigned char XOR_Check(char * str, unsigned char  len,unsigned char checksum);
unsigned char Sum_Get(char *dat,char len);
unsigned short CRC16Calculate(unsigned char *buff, unsigned char len);




#endif
