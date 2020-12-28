/**********************************************************************
��Ȩ���У�	���ش��¿Ƽ���2017.
��		����	http://www.miaowlabs.com
��		����	https://miaowlabs.taobao.com/
�� �� ��: 	bsp.c
��    ��:   ����ʵ����
��		��:   3.00
�������:   2017.03.01
��		Ҫ: 	


***********************************************************************/

#include "stdio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "common.h"
#include "MPU6050.h"
#include "oled.h"
#include "bsp.h"
#include "ADC.h"
#include "usart.h"
#include "motor.h"
#include "I2C.h"
#include "ultrasonic.h"
#include "infrare.h"
#include "hc05.h"

/* 
	ʹ��SWD�� ʧ��JTAG
	PB3,PB4,PA15����ͨIOʹ�ã�����ʧ��JTAG�� 
*/
void SWDConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
}



void LEDInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //KKDD B12->C13

  /*GPIOB Configuration*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);// KKDD B->C
}
void LEDToggle(void)
{
	static char flag=0;
	if(flag==0)
	{
		flag = 1;
		GPIO_SetBits(GPIOC, GPIO_Pin_13); //KKDD B12->C13
	}
	else{
		flag = 0;
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);//KKDD B12->C13
	}
}
/*
	��ȡ��Ƭ��12�ֽ� ID
*/
#define UniqueID_Addr 0x1FFFF7E8U
void GetUniqueID(char* ID)
{
	char i;
	char *p = (char*)UniqueID_Addr; 
	for(i=0; i<12; i++){
		*ID++ = *p++;
	}
}


/*
	������������
*/
void SetBlueToothName(void)
{
	char temp[32];
	char check;
	GetUniqueID(temp);
	check = XOR_Get(temp, 12);
	sprintf(temp, "\r\nAT+NAME=1+1>2%04X\r\n",(int)check);
	Uart1SendStr(temp);// KKDD 3->1
	delay_ms(100);
}


/*
	bsp��ʼ��
*/
void BspInit(void)
{
	SWDConfig();
	
	//KKDD// ADCInit();				//ADC��ʼ��

	//USART1Init(38400);			//����1��ʼ��-�װ�Ԥ�����ؼ�������
	//HC05_Init();						//Bluetooth name config
	//KKDD// USART3Init(1);			//����3��ʼ��-��������  KKDD//
	//KKDD// TIM1_Cap_Init();		//TIM1��ʼ��-���ڳ��������湦��
	TIM3_PWM_Init(); 		//PWM��ʼ��
	TIM2_Encoder_Init();	//TIM2���������ʼ��-���ڲ���
	TIM4_Encoder_Init();	//TIM4���������ʼ��-���ڲ���
	
	i2cInit();	 			//I2C��ʼ��
	
	//KKDD// InfraredIOInit();		//����IO�ڳ�ʼ��

	//KKDD// OLED_Init();			//OLED��ʼ�� KKDD //
	MPU6050_Init();		    //OLED��ʼ��
	
	LEDInit();				//ָʾ�Ƴ�ʼ��
	
	//KKDD// UltraSelfCheck();							//	����ģ�鿪���Լ�
	//KKDD// InfrareSelfCheck();						//  ����ģ�鿪���Լ�
	
	delay_ms(500);			//��ʱ0.5s���ȴ�����ģ������
	//KKDD// Uart3SendStr("\r\nUART3 AT+BAUD8\r\n"); //�����������ڲ�����Ϊ115200 ( ԭ������9600 )  KKDD 3->1
	//Uart1SendStr("\r\nUART1 AT+BAUD8\r\n"); //�����������ڲ�����Ϊ115200 ( ԭ������9600 )  KKDD 3->1
	//KKDD// USART3Init(1);					//����UART3������Ϊ115200  KKDD//
	USART1Init(115200);
	//Uart1SendStr("\r\n115200 BAUD\r\n");
	//SetBlueToothName();		//��������ģ������
}

