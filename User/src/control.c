/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	main.c
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 
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

unsigned char g_cMotorDisable = 0;//ֵ����0ʱ�������ת��������ֹͣת��


int g_iGravity_Offset = 0;

/******������Ʋ���******/
float g_fSpeedControlOut;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;

/******�ٶȿ��Ʋ���******/

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

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*/
PID_t g_tCarAnglePID={30.24, 0, 10.9};//KKDD//{19, 0, 18}
PID_t g_tCarSpeedPID={21.26, 0.11, 0};	//KKDD// {11.11, 0.11, 0}

/******�������Ʋ���******/
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
** ��������: SpeedInnerControl
** ��������: PID motor control
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   1+1>2
** ��  ����  
** �ա���:   
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
** ��������: CarUpstandInit
** ��������: ȫ�ֱ�����ʼ������
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ�
** �ա���:   2014��08��01��
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
** ��������: AbnormalSpinDetect
** ��������: ���ת���쳣���      
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �ա���:   2017��4��26��
***************************************************************/

void AbnormalSpinDetect(short leftSpeed,short rightSpeed)
{
	static unsigned short count = 0;
	
	//�ٶ�����Ϊ0ʱ��⣬���򲻼��
	if(g_iCarSpeedSet==0)
	{
		if(((leftSpeed>60)&&(rightSpeed>60)&&(g_fCarAngle > -30) && (g_fCarAngle < 30))
			||((leftSpeed<-60)&&(rightSpeed<-60))&&(g_fCarAngle > -30) && (g_fCarAngle < 30))
		{// ���ҵ��ת�ٴ���60��������ͬ������ʱ�䳬��250ms���ҳ���ǶȲ�����30�ȣ����ж�Ϊ���տ�ת
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
** ��������: LandingDetect
** ��������: С���ŵؼ��      
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �ա���:   2017��4��26��
***************************************************************/
void LandingDetect(void)
{
	static float lastCarAngle = 0;
	static unsigned short count = 0,count1 = 0;
	
	if(AbnormalSpinFlag == 0)return;
	
	// С���Ƕ�5��~-5���������
	if((g_fCarAngle > -5) && (g_fCarAngle < 5))
	{
		count1++;
		if(count1 >= 50)
		{//ÿ��250ms�ж�һ��С���Ƕȱ仯�����仯��С��0.8������-0.8���ж�ΪС����ֹ
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
** ��������: MotorManage
** ��������: ���ʹ��/ʧ�ܿ���      
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �ա���:   2017��4��26��
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
** ��������: SetMotorVoltageAndDirection
** ��������: ���ת�ټ�������ƺ���             
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ�
** �ա���:   2018��08��27��
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
** ��������: MotorOutput
** ��������: ����������
             ��ֱ�����ơ��ٶȿ��ơ�������Ƶ���������е���,����
			 �����������������������������
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ� 
** �ա���:   2014��08��01��
***************************************************************/
void MotorOutput(void)
{
	g_fLeftMotorOut  = g_fAngleControlOut - g_fSpeedControlOut- g_fBluetoothDirection ;	
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut+ g_fBluetoothDirection ;


	/*������������*/
	if((int)g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if((int)g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if((int)g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if((int)g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*������ʹ�����ֹ����PWM��Χ*/			
	if((int)g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if((int)g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if((int)g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if((int)g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;
	
    SetMotorVoltageAndDirection((int)g_fLeftMotorOut,(int)g_fRightMotorOut);
}



void GetMotorPulse(void)  //�ɼ�����ٶ�����
{ 	
  g_s16LeftMotorPulse = (short)TIM2->CNT;
	TIM2->CNT = 0;//KKDD 0->1248
  g_s16RightMotorPulse = -(short)TIM4->CNT;
	TIM4->CNT = 0;//KKDD 0->1248
	//g_s16RightMotorPulse = -g_s16RightMotorPulse;
	//KKDD//g_s16LeftMotorPulse = TIM_GetCounter(TIM2);     
  //KKDD//g_s16RightMotorPulse= -TIM_GetCounter(TIM4);
  //KKDD//TIM2->CNT = 0;
  //KKDD//TIM4->CNT = 0;   //����

  g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
  g_s32RightMotorPulseSigma += g_s16RightMotorPulse; 
	
	g_iLeftTurnRoundCnt -= g_s16LeftMotorPulse;
	g_iRightTurnRoundCnt -= g_s16RightMotorPulse;

}

/***************************************************************
** ��������: AngleCalculate
** ��������: �ǶȻ����㺯��           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************Author: 1+1>2��Ȩ����**************************
***************************************************************/
void AngleCalculate(void)
{
	//-------���ٶ�--------------------------
	//����Ϊ��2gʱ�������ȣ�16384 LSB/g
    g_fGravityAngle = atan2(g_fAccel_y/16384.0,g_fAccel_z/16384.0) * 180.0 / 3.14;
	  g_fGravityAngle = g_fGravityAngle - g_iGravity_Offset;

	//-------���ٶ�-------------------------
	//��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	g_fGyro_x  = g_fGyro_x / 16.4;  //������ٶ�ֵ			   
	g_fGyroAngleSpeed = g_fGyro_x;	
	
	//-------�����˲�---------------
	g_fCarAngle = 0.98 * (g_fCarAngle + g_fGyroAngleSpeed * 0.005) + 0.02 *	g_fGravityAngle;
}
/***************************************************************
** Author: 1+1>2
** �ա�  ��: 2018��08��27��
** ��������: AngleControl
** ��������: �ǶȻ����ƺ���           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************Author: 1+1>2��Ȩ����**************************
***************************************************************/
void AngleControl(void)	 
{
	g_fAngleControlOut =  (CAR_ANGLE_SET-g_fCarAngle) * g_tCarAnglePID.P *5 + \
	(CAR_ANGLE_SPEED_SET-g_fGyroAngleSpeed) * (g_tCarAnglePID.D /10);
}



/***************************************************************
** ��������: SpeedControl
** ��������: �ٶȻ����ƺ���
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ�
** �ա���:   2014��08��01��
***************************************************************/

void SpeedControl(void)
{
  	float fP,fI;   	
	float fDelta;
	
	
	g_fCarSpeed = (g_s32LeftMotorPulseSigma  + g_s32RightMotorPulseSigma ) * 0.5 ;
  g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;	  //ȫ�ֱ��� ע�⼰ʱ����
    	
	g_fCarSpeed = 0.7 * g_fCarSpeedOld + 0.3 * g_fCarSpeed ;//��ͨ�˲���ʹ�ٶȸ�ƽ��
	g_fCarSpeedOld = g_fCarSpeed;

	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;   
	
	fP = fDelta * (g_tCarSpeedPID.P);
  fI = fDelta * (g_tCarSpeedPID.I/10.0);

	g_fCarPosition += fI;
	g_fCarPosition += g_fBluetoothSpeed;	  
	
//������������
	if((s16)g_fCarPosition > CAR_POSITION_MAX)    g_fCarPosition = CAR_POSITION_MAX;
	if((s16)g_fCarPosition < CAR_POSITION_MIN)    g_fCarPosition = CAR_POSITION_MIN;
	
	g_fSpeedControlOutOld = g_fSpeedControlOutNew;
  g_fSpeedControlOutNew = fP + g_fCarPosition;
}
/***************************************************************
** ��������: SpeedControlOutput
** ��������: �ٶȻ������������-�ֶಽ��αƽ���������������ܽ���ֱ�����ĸ��Ž��͡�
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ�
** �ա���:   2014��08��01��
***************************************************************/
void SpeedControlOutput(void)
{
  float fValue;
  fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld ;
  g_fSpeedControlOut = fValue * (g_u8SpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld; 
}


/***************************************************************
** ��������: Scale
** ��������: ���̹�һ������
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ�
** �ա���:   2014��08��01��
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
** ��������: Steer
** ��������: ң���ٶȼ���������
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** Author: 1+1>2
** �������ӣ�
** �ա���:   2014��08��01��
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
** �ա�  ��: 20160415
** ��������: UltraControl
** ��������: ����������           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************Author: 1+1>2��Ȩ����**************************/
void UltraControl(int mode)
{
	if(mode == 0)
	{
		if((Distance >= 0) && (Distance<= 12))
		{//����С��12cm�����
			Steer(0, -4);
		}
		else if((Distance> 18) && (Distance<= 30))	
		{//�������18cm��С��30��ǰ��
			Steer(0, 4);
		}
		else
			Steer(0, 0);
	}
	else if(mode == 1)
	{
		if((Distance >= 0) && (Distance<= 20))
		{//��ת1500�����������ת��Ƕ�ԼΪ80��
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
** ��������: TailingControl
** ��������: ����Ѱ��           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************Author: 1+1>2��Ȩ����**************************
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




