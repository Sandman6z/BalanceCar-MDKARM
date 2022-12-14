/**********************************************************************
Author:     1+1>2 Team
File:  	main.c
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	
***********************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"
#include "control.h"
#include "debug.h"
#include "communicate.h"
#include "dataflash.h"
#include "common.h"
#include "motor.h"
#include "display.h"
#include "bsp.h"
#include "ADC.h"
#include "ultrasonic.h"
#include "infrare.h"
#include "manage.h"
#include "hc05.h"


//秒级任务
void SecTask()
{
	if(SoftTimer[0])return;
	else{
		SoftTimer[0] = 1000;
	}
//KKDD//	if(g_nSpeedTarget<200)g_nSpeedTarget += 10; //KKDD//motor pulse position PID
	g_RunTime++; 			// 记录运行时间
//KKDD//	g_BatVolt = GetBatVoltage(); // 读取电池电压
	
	
	
	if(StatusFlag)ResponseStatus();
	
	LEDToggle();
	ShowHomePage(); 
	
/*KKDD HC05 test	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);					//KEY置高,进入AT模式
	delay_ms(10);
	//Uart1SendStr("AT+NAME=KKDD 1+1>2 HC05\r\n");
	delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
*/
}


/*
	主函数入门，另外，控制功能函数在stm32f10x_it.c执行文件的滴答定时器中断服务函数里循环执行。
*/
int main(void)
{	
	
	BspInit();				//初始化BSP

	//PIDInit(); 				//初始化PID
	
	CarUpstandInit(); 	//初始化系统参数
	
	SysTick_Init();			//初始化定时器	
	
	//KKDD// 	if(IsInfrareOK())
	//KKDD// 		g_iGravity_Offset = 1; //若果检测到悬挂红外模块，则更改偏移值。
	
	g_iGravity_Offset = -1;
	Steer(0,0);
	ShowHomePageInit();
	
	while (1)
	{
		
		SecTask();			//秒级任务

		if(SoftTimer[1] == 0)
		{// 每隔50ms 执行一次
			SoftTimer[1] = 20;
			ResponseIMU();	
			delay_ms(5);
			//DebugService();//KKDD 注释虚拟示波器调试模式			
			Parse(Uart1Buffer);////KKDD UART3->UART1
			//ShowHomePage();
			
		}		
		
/*KKDD注释超声避障和红外追踪模式
		if(SoftTimer[2] == 0)
		{
			SoftTimer[2] = 20;
			//ShowHomePage(); 
			Read_Distane();//超声波测距
			if(g_CarRunningMode == ULTRA_FOLLOW_MODE){
				if(IsUltraOK())UltraControl(0);	//超声波跟随模式
	 		}
			if(g_CarRunningMode == ULTRA_AVOID_MODE){
				if(IsUltraOK())UltraControl(1);	//超声波避障模式
	 		}
			else if(g_CarRunningMode == INFRARED_TRACE_MODE){
				TailingControl();//红外追踪模式
			}
		}			
*/
	}
}


/******************* (C) COPYRIGHT 1+1>2 Team *****END OF FILE************/
