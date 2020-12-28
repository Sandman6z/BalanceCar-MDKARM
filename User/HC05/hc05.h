#ifndef __HC05_H
#define __HC05_H	 
#include "stm32f10x_gpio.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-HC05����ģ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20140329
//�޸�LED�����ӣ�ԭ����PC5����ΪPA4���Լ���MiniSTM32������V3.0									  
//////////////////////////////////////////////////////////////////////////////////   

#define HC05_KEY1  	GPIO_SetBits(GPIOA,GPIO_Pin_1)//PA1 <- PCout(4) 	//��������KEY�ź�
#define HC05_KEY0 	GPIO_ResetBits(GPIOA,GPIO_Pin_1)//PA1 <-PCout(4) 	//��������KEY�ź�
#define HC05_LED  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//PA0 <- PAin(4)		//��������״̬�ź�
  
u8 HC05_Init(void);
void HC05_CFG_CMD(u8 *str);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);	   
#endif  
















