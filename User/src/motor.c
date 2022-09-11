/**********************************************************************
Author:     1+1>2 Team
������̳��
�������ӣ�
File:  	motor.c
@Copyright��by 1+1>2
Version��V1.0
Date��2020
@brief��2020 	���pwm�������������롢�������������ʼ��


***********************************************************************/

#include "stm32f10x.h"





void MOTOR_GPIO_Config(void)
{		
	/*????GPIO_InitTypeDef??????*/
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	
	GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4);
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);

}
/***********************************************************************

						PWMģʽ(�������)
						
***********************************************************************/
/*
 * ��������TIM3_GPIO_Init
 * ����  ������TIM3�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_GPIO_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM2 clock enable */
	//PCLK1����2��Ƶ����ΪTIM2��ʱ��Դ����72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_Mode_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	u16 CCR1_Val = 0;        
	u16 CCR2_Val = 0;
	u16 CCR3_Val = 0;
	u16 CCR4_Val = 0;
	//u16 PrescalerValue;
/* -----------------------------------------------------------------------
    TIM2 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 71999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 1 KHz.
		CC1 update rate = TIM2 counter clock / CCR1_Val
    TIM3 Channelx duty cycle = (TIM2_CCRx/ TIM2_ARR)* 100 = x%
    
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 3;//3;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	
  //KKDD// TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;   
  
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //
   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��4
   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);


  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM2 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��2	
}

void TIM3_PWM_Init(void)
{
	MOTOR_GPIO_Config();
	TIM3_GPIO_Init();
	TIM3_Mode_Init();	
}


/*
	���������ʼ��(�������)
*/
void TIM2_Encoder_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
/* Encoder unit connected to TIM2, 4X mode */    
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);        // ??JTAG??
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);            // ???TIM2?CH1?CH2?PA15?PB3

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
 
  /* TIM2 clock source enable */
 //KKDD RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Enable GPIOA, clock */
 //KKDD  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
 //KKDD  GPIO_StructInit(&GPIO_InitStructure);
  /* Configure PA.00,01 as encoder input */
 //KKDD  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 //KKDD  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 //KKDD  GPIO_Init(GPIOA, &GPIO_InitStructure);


  /* Enable the TIM3 Update Interrupt */
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 65536-1;//KKDD//0xFFFF;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//TIM_ICPolarity_Rising�����ز���
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, 
														 TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6; //���˲���
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	  //ʹ���ж�
  TIM_SetCounter(TIM2,0);//KKDD
  TIM_Cmd(TIM2, ENABLE);  	   //ʹ�ܶ�ʱ��3
}



/*
	���������ʼ��(�������)
*/
void TIM4_Encoder_Init(void)
{
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = 65536-1; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM4,0);
  TIM_Cmd(TIM4, ENABLE); 
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_ICInitTypeDef TIM_ICInitStructure;
//  
///* Encoder unit connected to TIM3, 4X mode */    
//  GPIO_InitTypeDef GPIO_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* TIM3 clock source enable */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//  /* Enable GPIOA, clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  
//  GPIO_StructInit(&GPIO_InitStructure);
//  /* Configure PA.06,07 as encoder input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
//  /* Enable the TIM3 Update Interrupt */
//  
//  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Timer configuration in Encoder mode */
//  TIM_DeInit(TIM4);
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // TIM4ʱ��Ԥ��Ƶֵ
//  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //�趨��ʱ����װֵ
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //����ʱ�ӷ�Ƶϵ��������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ 
//  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
// 
//  //TIM_ICPolarity_Rising�����ز���
//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
//                             TIM_ICPolarity_Rising, 
//														 TIM_ICPolarity_Rising); 
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 6; //���˲���
//  TIM_ICInit(TIM4, &TIM_ICInitStructure);
//  
// // Clear all pending interrupts
//  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	 //���TIM4�ĸ��±�־λ
//  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	  //����TIM4�ĸ��±�־λ
//  
//  TIM_Cmd(TIM4, ENABLE);  	   //ʹ�ܶ�ʱ��3
}

void TIM2_IRQHandler(void)
{ 		    		  			    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	    
}
void TIM4_IRQHandler(void)
{ 
	if(TIM4->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	
	//KKDD//TIM_ClearFlag(TIM4, TIM_FLAG_Update);	    
}




