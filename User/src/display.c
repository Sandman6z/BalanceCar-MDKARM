/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	display.c
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 	


***********************************************************************/

#include "stdio.h"
#include "oled-iic.h"
#include "bmp.h"
#include "control.h"
#include "ultrasonic.h"
#include "mpu6050.h"
#include "common.h"
#include "manage.h"
#include "bsp.h"

extern unsigned short BatVol;



/*
	��ʾlogo
*/
void ShowHomePageInit(void)
{
	OLED_Clear();
	delay_ms(1000);
	OLED_DrawBMP(0,0,128,8,LOGO);  //ͼƬ��ʾ
	delay_ms(2000);
	OLED_Clear();
}


/*
	oled��ҳˢ�º���
	������ˢ�£�����һ��ˢ��ʱ�����
*/

void ShowHomePage(void)
{
	unsigned char buff[32]={0};
	static char step = 0;
	step++;
	if(step >= 6)step = 0;

	//�ֲ�ִ�У����̵���ˢ��ʱ��
	if(step == 0){
		//OLED_ShowString(0, 0, "Mode: Complementary ",16);
	}

	if(step == 1){
		if(IsUltraOK())
			snprintf((char*)buff, 21,  "Distance:  %d(cm)   ", Distance);
		else
			snprintf((char*)buff, 21,  "Distance:  %s(cm)   ", "xx");
		
		//OLED_ShowString(0,0,buff,16);
	}

	if(step == 2){
		snprintf((char*)buff, 21,  "EnL:  %d",g_s16LeftMotorPulse);
		OLED_ShowString(0,2,buff,16);
	}
	if(step == 3){
		snprintf((char*)buff, 21, "EnR:  %d",g_s16RightMotorPulse);
		OLED_ShowString(0,4,buff,16);
	}
	
	if(step == 4){
		snprintf((char*)buff, 21, "Ang:  %0.1f", g_fCarAngle);
		OLED_ShowString(0, 6, buff,16);
	}
	if(step == 5){
		//snprintf((char*)buff, 21, "Battery:   %0.1f(V)      ", g_BatVolt/100.0);
		//OLED_ShowString(0, 8, buff,16);		
		}
}


