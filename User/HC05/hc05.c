#include "common.h"	 
#include "usart.h" 			 
//#include "usart2.h" 			 
#include "hc05.h" 
//#include "led.h" 
#include "string.h"	 
#include "math.h"
#include "communicate.h"

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

//��ʼ��ATK-HC05ģ��
//����ֵ:0,�ɹ�;1,ʧ��.
u8 HC05_Init(void)
{
	u8 retry=10,t;	  		 
	u8 temp=1;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	//ʹ��PORTA Cʱ��	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��A4
	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC4

	GPIO_SetBits(GPIOA,GPIO_Pin_1);
 	
	//USART1Init(9600);	//��ʼ������2Ϊ:9600,������.
	
	while(retry--)
	{
		HC05_KEY1;					//KEY�ø�,����ATģʽ
		delay_ms(50);
		//Uart1SendStr("AT\r\n");		//����AT����ָ��
		Uart1SendStr(" \r\nAT+NAME=HC05 Master by 1+1>2 team\r\n");//��ʱ�䰴ס����ģ��key���ϵ磬�ȴ�10���Ӻ������������óɹ�
		delay_ms(50);
		HC05_KEY0;					//KEY����,�˳�ATģʽ
		for(t=0;t<10;t++) 			//��ȴ�50ms,������HC05ģ��Ļ�Ӧ
		{
			if(ParseAT(Uart1Buffer))
			{
				temp = 0;
				break;
			}
				delay_ms(5);
		}		
		
		Uart1SendStr(" \r\nAT+UART=115200,0,0\r\n");
		delay_ms(50);
		HC05_KEY0;					//KEY����,�˳�ATģʽ
		for(t=0;t<10;t++) 			//��ȴ�50ms,������HC05ģ��Ļ�Ӧ
		{
			if(ParseAT(Uart1Buffer))
			{
				temp = 0;
				break;
			}
				delay_ms(5);
		}	
		
//		if(USART2_RX_STA&0X8000)	//���յ�һ��������
//		{
//			temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
//			USART2_RX_STA=0;			 
//			if(temp==4&&USART2_RX_BUF[0]=='O'&&USART2_RX_BUF[1]=='K')
//			{
//				temp=0;//���յ�OK��Ӧ
//				break;
//			}
//		}			    		
	}		    
	if(retry==0)temp=1;	//���ʧ��
	return temp;	 
}	 
//��ȡATK-HC05ģ��Ľ�ɫ
//����ֵ:0,�ӻ�;1,����;0XFF,��ȡʧ��.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		Uart1SendStr("AT+ROLE?\r\n");	//��ѯ��ɫ
		for(t=0;t<20;t++) 			//��ȴ�200ms,������HC05ģ��Ļ�Ӧ
		{
			delay_ms(10);
			if(ParseAT(Uart1Buffer))break;
		}		
		HC05_KEY0;					//KEY����,�˳�ATģʽ
		if(ParseAT(Uart1Buffer))	//���յ�һ��������
		{
//			temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
//			USART2_RX_STA=0;			 
//			if(temp==13&&USART2_RX_BUF[0]=='+')//���յ���ȷ��Ӧ����
//			{
				temp=Uart1Buffer[6]-'0';//�õ�����ģʽֵ
				break;
//			}
		}		
	}
	if(retry==0)temp=0XFF;//��ѯʧ��.
	return temp;
} 							   
//ATK-HC05��������
//�˺�����������ATK-HC05,�����ڽ�����OKӦ���ATָ��
//atstr:ATָ�.����:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"���ַ���
//����ֵ:0,���óɹ�;����,����ʧ��.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		Uart1SendStr((char*)atstr);	//����AT�ַ���
		HC05_KEY0;					//KEY����,�˳�ATģʽ
		for(t=0;t<20;t++) 			//��ȴ�100ms,������HC05ģ��Ļ�Ӧ
		{
			if(ParseAT(Uart1Buffer))break;
			delay_ms(5);
		}		
		if(ParseAT(Uart1Buffer))	//���յ�һ��������
			{		
				temp=0;
				break;			 
			}		
	}
	if(retry==0)temp=0XFF;//����ʧ��.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//ͨ���ú���,��������USMART,���Խ��ڴ���2�ϵ�ATK-HC05ģ��
//str:���.(����ע�ⲻ����Ҫ������س���)
void HC05_CFG_CMD(u8 *str)
{					  
//	u8 temp;
	u8 t;		  
	HC05_KEY1;						//KEY�ø�,����ATģʽ
	delay_ms(10);
	Uart1SendStr((char*)str); //����ָ��
	for(t=0;t<50;t++) 				//��ȴ�500ms,������HC05ģ��Ļ�Ӧ
	{
		if(ParseAT(Uart1Buffer))break;
		delay_ms(10);
	}									    
	HC05_KEY0;						//KEY����,�˳�ATģʽ
//	if(USART2_RX_STA&0X8000)		//���յ�һ��������
//	{
//		temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
//		USART2_RX_STA=0;
//		USART2_RX_BUF[temp]=0;		//�ӽ�����		 
//		printf("\r\n%s",USART2_RX_BUF);//���ͻ�Ӧ���ݵ�����1
//	} 				 
}














