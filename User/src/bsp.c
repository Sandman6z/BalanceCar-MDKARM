/**********************************************************************
版权所有：	喵呜创新科技，2017.
官		网：	http://www.miaowlabs.com
淘		宝：	https://miaowlabs.taobao.com/
文 件 名: 	bsp.c
作    者:   喵呜实验室
版		本:   3.00
完成日期:   2017.03.01
概		要: 	


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
	使能SWD， 失能JTAG
	PB3,PB4,PA15做普通IO使用（必须失能JTAG） 
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // 复用推挽输出
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
	读取单片机12字节 ID
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
	设置蓝牙名称
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
	bsp初始化
*/
void BspInit(void)
{
	SWDConfig();
	
	//KKDD// ADCInit();				//ADC初始化

	//USART1Init(38400);			//串口1初始化-底板预留下载及调试用
	//HC05_Init();						//Bluetooth name config
	//KKDD// USART3Init(1);			//串口3初始化-用于蓝牙  KKDD//
	//KKDD// TIM1_Cap_Init();		//TIM1初始化-用于超声波跟随功能
	TIM3_PWM_Init(); 		//PWM初始化
	TIM2_Encoder_Init();	//TIM2正交解码初始化-用于测速
	TIM4_Encoder_Init();	//TIM4正交解码初始化-用于测速
	
	i2cInit();	 			//I2C初始化
	
	//KKDD// InfraredIOInit();		//红外IO口初始化

	//KKDD// OLED_Init();			//OLED初始化 KKDD //
	MPU6050_Init();		    //OLED初始化
	
	LEDInit();				//指示灯初始化
	
	//KKDD// UltraSelfCheck();							//	超声模块开机自检
	//KKDD// InfrareSelfCheck();						//  红外模块开机自检
	
	delay_ms(500);			//延时0.5s，等待蓝牙模块启动
	//KKDD// Uart3SendStr("\r\nUART3 AT+BAUD8\r\n"); //配置蓝牙串口波特率为115200 ( 原波特率9600 )  KKDD 3->1
	//Uart1SendStr("\r\nUART1 AT+BAUD8\r\n"); //配置蓝牙串口波特率为115200 ( 原波特率9600 )  KKDD 3->1
	//KKDD// USART3Init(1);					//更改UART3波特率为115200  KKDD//
	USART1Init(115200);
	//Uart1SendStr("\r\n115200 BAUD\r\n");
	//SetBlueToothName();		//配置蓝牙模块名称
}

