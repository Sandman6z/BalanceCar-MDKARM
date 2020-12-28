/**********************************************************************
��Ȩ���У�	���ش��¿Ƽ���2017.
��		����	http://www.miaowlabs.com
��		����	https://miaowlabs.taobao.com/
�� �� ��: 	main.c
��    ��:   ����ʵ����
��		��:   3.00
�������:   2017.03.01
��		Ҫ: 	

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


//�뼶����
void SecTask()
{
	if(SoftTimer[0])return;
	else{
		SoftTimer[0] = 1000;
	}
//KKDD//	if(g_nSpeedTarget<200)g_nSpeedTarget += 10; //KKDD//motor pulse position PID
	g_RunTime++; 			// ��¼����ʱ��
//KKDD//	g_BatVolt = GetBatVoltage(); // ��ȡ��ص�ѹ
	
	
	
	if(StatusFlag)ResponseStatus();
	
	LEDToggle();
/*KKDD HC05 test	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);					//KEY�ø�,����ATģʽ
	delay_ms(10);
	//Uart1SendStr("AT+NAME=KKDD 1+1>2 HC05\r\n");
	delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
*/
}


/*
	���������ţ����⣬���ƹ��ܺ�����stm32f10x_it.cִ���ļ��ĵδ�ʱ���жϷ�������ѭ��ִ�С�
*/
int main(void)
{	
	
	BspInit();				//��ʼ��BSP

	//PIDInit(); 				//��ʼ��PID
	
	CarUpstandInit(); 	//��ʼ��ϵͳ����
	
	SysTick_Init();			//��ʼ����ʱ��	
	
	//KKDD// 	if(IsInfrareOK())
	//KKDD// 		g_iGravity_Offset = 1; //������⵽���Һ���ģ�飬�����ƫ��ֵ��
	
	g_iGravity_Offset = -1;
	Steer(0,0);
	//KKDD// ShowHomePageInit();
	
	while (1)
	{
		
		SecTask();			//�뼶����

		if(SoftTimer[1] == 0)
		{// ÿ��50ms ִ��һ��
			SoftTimer[1] = 20;
			ResponseIMU();	
			delay_ms(5);
			//DebugService();//KKDD ע������ʾ��������ģʽ			
			Parse(Uart1Buffer);////KKDD UART3->UART1
			
		}			
/*KKDDע�ͳ������Ϻͺ���׷��ģʽ
		if(SoftTimer[2] == 0)
		{
			SoftTimer[2] = 20;
			//ShowHomePage(); 
			Read_Distane();//���������
			if(g_CarRunningMode == ULTRA_FOLLOW_MODE){
				if(IsUltraOK())UltraControl(0);	//����������ģʽ
	 		}
			if(g_CarRunningMode == ULTRA_AVOID_MODE){
				if(IsUltraOK())UltraControl(1);	//����������ģʽ
	 		}
			else if(g_CarRunningMode == INFRARED_TRACE_MODE){
				TailingControl();//����׷��ģʽ
			}
		}			
*/
	}
}


/******************* (C) COPYRIGHT 2016 MiaowLabs Team *****END OF FILE************/


