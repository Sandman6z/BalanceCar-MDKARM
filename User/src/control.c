/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	main.c
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 
***********************************************************************/
#include "math.h"
#include "stdio.h"
#include "control.h"
#include "debug.H"
#include "MPU6050.H"
#include "communicate.h"
#include "bsp.h"
#include "ultrasonic.h"
#include "infrare.h"


unsigned char g_u8MainEventCount;
unsigned char g_u8SpeedControlCount;
unsigned char g_u8SpeedControlPeriod;
unsigned char g_u8DirectionControlPeriod;
unsigned char g_u8DirectionControlCount;

unsigned char g_cMotorDisable = 0;//值等于0时电机正常转动，否则停止转动


int g_iGravity_Offset = 0;

/******电机控制参数******/
float g_fSpeedControlOut;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;

/******速度控制参数******/

short  g_s16LeftMotorPulse;
short  g_s16RightMotorPulse;

int  g_s32LeftMotorPulseOld;
int  g_s32RightMotorPulseOld;
int  g_s32LeftMotorPulseSigma;
int  g_s32RightMotorPulseSigma;

float g_fCarSpeed;
float g_iCarSpeedSet;
float g_fCarSpeedOld;
float g_fCarPosition;

/*-----角度环和速度环PID控制参数-----*/
PID_t g_tCarAnglePID={30.24, 0, 10.9};//KKDD//{19, 0, 18}
PID_t g_tCarSpeedPID={21.26, 0.11, 0};	//KKDD// {11.11, 0.11, 0}

/******蓝牙控制参数******/
float g_fBluetoothSpeed;
float g_fBluetoothDirection;
float g_fBluetoothDirectionOld;
float g_fBluetoothDirectionNew;
float g_fBluetoothDirectionOut;

float g_fCarAngle;         	//
float g_fGyroAngleSpeed;		//     			
float g_fGravityAngle;			//


int g_iLeftTurnRoundCnt = 0;
int g_iRightTurnRoundCnt = 0;

static int AbnormalSpinFlag = 0;

/***************************************************************
** 函数名称: SpeedInnerControl
** 功能描述: PID motor control
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   1+1>2
** 淘  宝：  
** 日　期:   
***************************************************************/
//unsigned int g_nLeftMotorPulse;

int g_nSpeedTarget = 0;
int g_nLSpeedpwm = 0;
int g_nRSpeedpwm = 0;

//int g_nLeftMotorOutput;


int LSpeedInnerControl(int nPulse,int nTarget) //KKDD ADD
{
		static int nErrorPrev = 0;//
		static int nPwm = 0;//PWM total
		static int nPwmBais = 0;//PWM increment,
    
		int nError;
		float fP = 3.8, fI = 1;

    nError = nTarget-nPulse;//*(g_u8SpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD

    nPwmBais = fP * (nError - nErrorPrev)+ fI * nError;

    nErrorPrev = nError;

    nPwm += nPwmBais;

    if(nPwm > MOTOR_OUT_MAX) nPwm = MOTOR_OUT_MAX;
    if(nPwm < MOTOR_OUT_MIN) nPwm = MOTOR_OUT_MIN;

//    OutData[0]=(float)nPulse;//?????
//    OutData[1]=(float)nTarget ;//?????
//    OutData[2]=(float)nPwm;//PWM???

    return nPwm;
}

int RSpeedInnerControl(int nPulse,int nTarget) //KKDD ADD
{
		static int nErrorPrev = 0;//
		static int nPwm = 0;//PWM total
		static int nPwmBais = 0;//PWM increment,
    
		int nError;
		float fP = 3.8, fI = 1;

    nError = nPulse-nTarget;//*(g_u8SpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD

    nPwmBais = fP * (nError - nErrorPrev)+ fI * nError;

    nErrorPrev = nError;

    nPwm += nPwmBais;

    if(nPwm > MOTOR_OUT_MAX) nPwm = MOTOR_OUT_MAX;
    if(nPwm < MOTOR_OUT_MIN) nPwm = MOTOR_OUT_MIN;

//    OutData[0]=(float)nPulse;//?????
//    OutData[1]=(float)nTarget ;//?????
//    OutData[2]=(float)nPwm;//PWM???

    return nPwm;
}




/***************************************************************
** 函数名称: CarUpstandInit
** 功能描述: 全局变量初始化函数
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接：
** 日　期:   2014年08月01日
***************************************************************/
void CarUpstandInit(void)
{
	//g_iAccelInputVoltage_X_Axis = g_iGyroInputVoltage_Y_Axis = 0;
	g_s16LeftMotorPulse = g_s16RightMotorPulse = 0;
	g_s32LeftMotorPulseOld = g_s32RightMotorPulseOld = 0;
	g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;

	g_fCarSpeed = g_fCarSpeedOld = 0;
	g_fCarPosition = 0;
	g_fCarAngle    = 0;
	g_fGyroAngleSpeed = 0;
	g_fGravityAngle   = 0;

	g_fAngleControlOut = g_fSpeedControlOut = g_fBluetoothDirectionOut = 0;
	g_fLeftMotorOut    = g_fRightMotorOut   = 0;
	g_fBluetoothSpeed  = g_fBluetoothDirection = 0;
	g_fBluetoothDirectionNew = g_fBluetoothDirectionOld = 0;

  g_u8MainEventCount=0;
	g_u8SpeedControlCount=0;
 	g_u8SpeedControlPeriod=0;
}


/***************************************************************
** 函数名称: AbnormalSpinDetect
** 功能描述: 电机转速异常检测      
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 日　期:   2017年4月26日
***************************************************************/

void AbnormalSpinDetect(short leftSpeed,short rightSpeed)
{
	static unsigned short count = 0;
	
	//速度设置为0时检测，否则不检测
	if(g_iCarSpeedSet==0)
	{
		if(((leftSpeed>60)&&(rightSpeed>60)&&(g_fCarAngle > -30) && (g_fCarAngle < 30))
			||((leftSpeed<-60)&&(rightSpeed<-60))&&(g_fCarAngle > -30) && (g_fCarAngle < 30))
		{// 左右电机转速大于60、方向相同、持续时间超过250ms，且车身角度不超过30度，则判断为悬空空转
			count++;
			if(count>50){
				count = 0;
				AbnormalSpinFlag = 1;
			}
		}
		else{
			count = 0;
		}
	}
	else{
		count = 0;
	}
}

/***************************************************************
** 函数名称: LandingDetect
** 功能描述: 小车着地检测      
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 日　期:   2017年4月26日
***************************************************************/
void LandingDetect(void)
{
	static float lastCarAngle = 0;
	static unsigned short count = 0,count1 = 0;
	
	if(AbnormalSpinFlag == 0)return;
	
	// 小车角度5°~-5°启动检测
	if((g_fCarAngle > -5) && (g_fCarAngle < 5))
	{
		count1++;
		if(count1 >= 50)
		{//每隔250ms判断一次小车角度变化量，变化量小于0.8°或大于-0.8°判断为小车静止
			count1 = 0;
			if(((g_fCarAngle - lastCarAngle) < 0.8) && ((g_fCarAngle - lastCarAngle) > -0.8))
			{
				count++;
				if(count >= 4){
					count = 0;
					count1 = 0;
					g_fCarPosition = 0;
					AbnormalSpinFlag = 0;
				}
			}
			else{
				count = 0;
			}
			lastCarAngle = g_fCarAngle;
		}
	}
	else
	{
		count1 = 0;
		count = 0;
	}
}

/***************************************************************
** 函数名称: MotorManage
** 功能描述: 电机使能/失能控制      
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 日　期:   2017年4月26日
***************************************************************/
void MotorManage(void)
{

	AbnormalSpinDetect(g_s16LeftMotorPulse, g_s16RightMotorPulse);
		
	LandingDetect();
	
	if(AbnormalSpinFlag)
	{	
		g_cMotorDisable |= (0x01<<1);
	}
	else
	{
		g_cMotorDisable &= ~(0x01<<1);
	}
	
	if(g_fCarAngle > 30 || g_fCarAngle < (-30))
	{
		g_cMotorDisable |= (0x01<<2);
	}
	else
	{
		g_cMotorDisable &= ~(0x01<<2);
	}
	
}

/***************************************************************
** 函数名称: SetMotorVoltageAndDirection
** 功能描述: 电机转速及方向控制函数             
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接：
** 日　期:   2018年08月27日
***************************************************************/
void SetMotorVoltageAndDirection(int i16LeftVoltage,int i16RightVoltage)
{
	  if(i16LeftVoltage<0)
    {	
			GPIO_SetBits(GPIOA, GPIO_Pin_3 );				    
      GPIO_ResetBits(GPIOA, GPIO_Pin_4 );
      i16LeftVoltage = (-i16LeftVoltage);
    }
    else 
    {	
      GPIO_SetBits(GPIOA, GPIO_Pin_4 );				    
      GPIO_ResetBits(GPIOA, GPIO_Pin_3 ); 
    }

    if(i16RightVoltage<0)
    {	
     	GPIO_SetBits(GPIOB, GPIO_Pin_0 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_1 );
      i16RightVoltage = (-i16RightVoltage);
    }
    else
    {
			GPIO_SetBits(GPIOB, GPIO_Pin_1 );				    
			GPIO_ResetBits(GPIOB, GPIO_Pin_0 );	      
    }

	if(i16RightVoltage > MOTOR_OUT_MAX)  
	{
		i16RightVoltage = MOTOR_OUT_MAX;
	}
	if(i16LeftVoltage > MOTOR_OUT_MAX)
	{
	   i16LeftVoltage = MOTOR_OUT_MAX;
	}  
	
	if(g_cMotorDisable)
	{
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,0); 
	}
	else
	{
		TIM_SetCompare1(TIM3,i16LeftVoltage);//KKDD//Right-> Left
		TIM_SetCompare2(TIM3,i16RightVoltage);//KKDD// Left->Right
	}
}


/***************************************************************
** 函数名称: MotorOutput
** 功能描述: 电机输出函数
             将直立控制、速度控制、方向控制的输出量进行叠加,并加
			 入死区常量，对输出饱和作出处理。
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接： 
** 日　期:   2014年08月01日
***************************************************************/
void MotorOutput(void)
{
	g_fLeftMotorOut  = g_fAngleControlOut - g_fSpeedControlOut- g_fBluetoothDirection ;	
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut+ g_fBluetoothDirection ;


	/*增加死区常数*/
	if((int)g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if((int)g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if((int)g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if((int)g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*输出饱和处理，防止超出PWM范围*/			
	if((int)g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if((int)g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if((int)g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if((int)g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;
	
    SetMotorVoltageAndDirection((int)g_fLeftMotorOut,(int)g_fRightMotorOut);
}



void GetMotorPulse(void)  //采集电机速度脉冲
{ 	
  g_s16LeftMotorPulse = (short)TIM2->CNT;
	TIM2->CNT = 0;//KKDD 0->1248
  g_s16RightMotorPulse = -(short)TIM4->CNT;
	TIM4->CNT = 0;//KKDD 0->1248
	//g_s16RightMotorPulse = -g_s16RightMotorPulse;
	//KKDD//g_s16LeftMotorPulse = TIM_GetCounter(TIM2);     
  //KKDD//g_s16RightMotorPulse= -TIM_GetCounter(TIM4);
  //KKDD//TIM2->CNT = 0;
  //KKDD//TIM4->CNT = 0;   //清零

  g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
  g_s32RightMotorPulseSigma += g_s16RightMotorPulse; 
	
	g_iLeftTurnRoundCnt -= g_s16LeftMotorPulse;
	g_iRightTurnRoundCnt -= g_s16RightMotorPulse;

}

/***************************************************************
** 函数名称: AngleCalculate
** 功能描述: 角度环计算函数           
** 输　  入:   
** 输　  出:   
** 备    注: 
********************Author: 1+1>2版权所有**************************
***************************************************************/
void AngleCalculate(void)
{
	//-------加速度--------------------------
	//量程为±2g时，灵敏度：16384 LSB/g
    g_fGravityAngle = atan2(g_fAccel_y/16384.0,g_fAccel_z/16384.0) * 180.0 / 3.14;
	  g_fGravityAngle = g_fGravityAngle - g_iGravity_Offset;

	//-------角速度-------------------------
	//范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)
	g_fGyro_x  = g_fGyro_x / 16.4;  //计算角速度值			   
	g_fGyroAngleSpeed = g_fGyro_x;	
	
	//-------互补滤波---------------
	g_fCarAngle = 0.98 * (g_fCarAngle + g_fGyroAngleSpeed * 0.005) + 0.02 *	g_fGravityAngle;
}
/***************************************************************
** Author: 1+1>2
** 日　  期: 2018年08月27日
** 函数名称: AngleControl
** 功能描述: 角度环控制函数           
** 输　  入:   
** 输　  出:   
** 备    注: 
********************Author: 1+1>2版权所有**************************
***************************************************************/
void AngleControl(void)	 
{
	g_fAngleControlOut =  (CAR_ANGLE_SET-g_fCarAngle) * g_tCarAnglePID.P *5 + \
	(CAR_ANGLE_SPEED_SET-g_fGyroAngleSpeed) * (g_tCarAnglePID.D /10);
}



/***************************************************************
** 函数名称: SpeedControl
** 功能描述: 速度环控制函数
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接：
** 日　期:   2014年08月01日
***************************************************************/

void SpeedControl(void)
{
  	float fP,fI;   	
	float fDelta;
	
	
	g_fCarSpeed = (g_s32LeftMotorPulseSigma  + g_s32RightMotorPulseSigma ) * 0.5 ;
  g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;	  //全局变量 注意及时清零
    	
	g_fCarSpeed = 0.7 * g_fCarSpeedOld + 0.3 * g_fCarSpeed ;//低通滤波，使速度更平滑
	g_fCarSpeedOld = g_fCarSpeed;

	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;   
	
	fP = fDelta * (g_tCarSpeedPID.P);
  fI = fDelta * (g_tCarSpeedPID.I/10.0);

	g_fCarPosition += fI;
	g_fCarPosition += g_fBluetoothSpeed;	  
	
//积分上限设限
	if((s16)g_fCarPosition > CAR_POSITION_MAX)    g_fCarPosition = CAR_POSITION_MAX;
	if((s16)g_fCarPosition < CAR_POSITION_MIN)    g_fCarPosition = CAR_POSITION_MIN;
	
	g_fSpeedControlOutOld = g_fSpeedControlOutNew;
  g_fSpeedControlOutNew = fP + g_fCarPosition;
}
/***************************************************************
** 函数名称: SpeedControlOutput
** 功能描述: 速度环控制输出函数-分多步逐次逼近最终输出，尽可能将对直立环的干扰降低。
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接：
** 日　期:   2014年08月01日
***************************************************************/
void SpeedControlOutput(void)
{
  float fValue;
  fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld ;
  g_fSpeedControlOut = fValue * (g_u8SpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld; 
}


/***************************************************************
** 函数名称: Scale
** 功能描述: 量程归一化处理
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接：
** 日　期:   2014年08月01日
***************************************************************/
float Scale(float input, float inputMin, float inputMax, float outputMin, float outputMax) { 
  float output;
  if (inputMin < inputMax)
    output = (input - inputMin) / ((inputMax - inputMin) / (outputMax - outputMin));
  else
    output = (inputMin - input) / ((inputMin - inputMax) / (outputMax - outputMin));
  if (output > outputMax)
    output = outputMax;
  else if (output < outputMin)
    output = outputMin;
  return output;
}

/***************************************************************
** 函数名称: Steer
** 功能描述: 遥控速度及方向处理函数
** 输　入:   
** 输　出:   
** 全局变量: 
** Author: 1+1>2
** 购买链接：
** 日　期:   2014年08月01日
***************************************************************/
void Steer(float direct, float speed)
{
	if(direct > 0)
		g_fBluetoothDirection = Scale(direct, 0, 10, 0, 400); //input < inputmin then output = (input - inputMin) / ((inputMax - inputMin) / (outputMax - outputMin))
	else
		g_fBluetoothDirection = -Scale(direct, 0, -10, 0, 400);

	if(speed > 0)
		g_iCarSpeedSet = Scale(speed, 0, 10, 0, 175);
	else
		g_iCarSpeedSet = -Scale(speed, 0, -10, 0, 175);

}

/***************************************************************
** 日　  期: 20160415
** 函数名称: UltraControl
** 功能描述: 超声波跟随           
** 输　  入:   
** 输　  出:   
** 备    注: 
********************Author: 1+1>2版权所有**************************/
void UltraControl(int mode)
{
	if(mode == 0)
	{
		if((Distance >= 0) && (Distance<= 12))
		{//距离小于12cm则后退
			Steer(0, -4);
		}
		else if((Distance> 18) && (Distance<= 30))	
		{//距离大于18cm且小于30则前进
			Steer(0, 4);
		}
		else
			Steer(0, 0);
	}
	else if(mode == 1)
	{
		if((Distance >= 0) && (Distance<= 20))
		{//右转1500个脉冲计数，转弯角度约为80度
			Steer(5, 0);
			g_iLeftTurnRoundCnt = 1500;
			g_iRightTurnRoundCnt = -1500;
		}
		if((g_iLeftTurnRoundCnt < 0)&&(g_iRightTurnRoundCnt > 0))
		{
			Steer(0, 4);
		}
	}
}

/***************************************************************
** 函数名称: TailingControl
** 功能描述: 红外寻迹           
** 输　  入:   
** 输　  出:   
** 备    注: 
********************Author: 1+1>2版权所有**************************
***************************************************************/
void TailingControl(void)
{
#if INFRARE_DEBUG_EN > 0
	char buff[32];	
#endif
	char result;
	float direct = 0;
	float speed = 0;

	result = InfraredDetect();
	
	if(result & infrared_channel_Lc)
		direct = -10;
	else if(result & infrared_channel_Lb)
		direct = -6;
	else if(result & infrared_channel_La)
		direct = -4;
	else if(result & infrared_channel_Rc)
		direct = 10;
	else if(result & infrared_channel_Rb)
		direct = 6;
	else if(result & infrared_channel_Ra)
		direct = 4;
	else
		direct = 0.0;

	speed = 4;

	Steer(direct, speed);

#if INFRARE_DEBUG_EN > 0
	sprintf(buff, "Steer:%d, Speed:%d\r\n",(int)direct,  (int)speed);
	DebugOutStr(buff);
#endif
}




