/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/

#ifndef __USART_H
#define __USART_H



#define Uart1BufferSize 128 //KKDD 3->1

extern char Uart1Buffer[];			//蓝牙通信串口接收缓冲区 KKDD 3->1
extern unsigned char Uart1Index; //KKDD 3->1

void USART1Init(unsigned int baudrate);
void USART3Init(char baudrate);


void Uart1SendByte(char byte) ;
void Uart1SendBuff(char *buf, unsigned short len);
void Uart1SendStr(char * str);
	
void Uart3SendByte(char byte)  ;
void Uart3SendBuff(char *buf, unsigned short len);
void Uart3SendStr(char *str);

#define BluetoothOut(str) Uart1SendStr(str) //KKDD 3->1



#endif /* __USART1_H */

