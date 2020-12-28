/**********************************************************************
版权所有：	喵呜创新科技，2017.
官		网：	http://www.miaowlabs.com
淘		宝：	https://shop275516297.taobao.com/
文 件 名: 	.h
作    者:   喵呜实验室
版		本:   3.00
完成日期:   2017.03.01
概		要: 	




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

