/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include "control.h"
#include "mpu6050.h"
#include "usart.h"
#include "systick.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


void TIM1_UP_IRQHandler(void)  
{  
}


/*
	ϵͳ�δ�ʱ�жϣ��ж�����1ms
*/
void SysTick_Handler(void)
{  
	SoftTimerCountDown();			 //��ʱ��

///*KKDD USER CODE BEGIN SysTick_IRQn 0 */

//  g_u8MainEventCount++;
//	g_u8SpeedControlPeriod++; 
//  if(g_u8MainEventCount>=25)
//    {
//        g_u8MainEventCount=0;
//        GetMotorPulse();
//    }
//	else if(g_u8MainEventCount==1)
//    {
//        g_nLSpeedpwm = LSpeedInnerControl(g_s16LeftMotorPulse,g_nSpeedTarget);
//				g_nRSpeedpwm = RSpeedInnerControl(g_s16RightMotorPulse,g_nSpeedTarget);
//				g_u8SpeedControlPeriod=0;
//    }
//	else if(g_u8MainEventCount==2)
//    {
//			//SetMotorVoltageAndDirection(g_nSpeedTarget,g_nSpeedTarget);
//        
//			SetMotorVoltageAndDirection(g_nLSpeedpwm,g_nRSpeedpwm);
//    }

///*KKDD USER CODE END SysTick_IRQn 0 */
	
	g_u8MainEventCount++;

	g_u8SpeedControlPeriod++;     
	SpeedControlOutput();   		 //�ٶȻ��������������ÿ1msִ��һ��
	if(g_u8MainEventCount>=5)
	{
		g_u8MainEventCount=0;
		GetMotorPulse();			 //���������壨�ٶȣ�������ÿ5msִ��һ��
	}
	else if(g_u8MainEventCount==1)
	{
		MPU6050_Pose();				 //��ȡMPU6050���ݺ�����ÿ5msִ��һ��
		AngleCalculate();			 //�ǶȻ����㺯����ÿ5msִ��һ��
	}
	else if(g_u8MainEventCount==2)
	{
		AngleControl();				 //�ǶȻ����ƺ�����ÿ5msִ��һ��

	}
	else if(g_u8MainEventCount==3)
	{
		g_u8SpeedControlCount++;
    	if(g_u8SpeedControlCount >= 5)//25ms
    	{		
      		SpeedControl();          //��ģ�ٶȿ��ƺ�����ÿ25ms����һ��
      		g_u8SpeedControlCount=0;
			    g_u8SpeedControlPeriod=0;
    	}
	}
	else if(g_u8MainEventCount==4)
	{
		MotorManage();			//���ʹ��/ʧ�ܿ��ƺ�����ÿ5msִ��һ��
		MotorOutput();	 		//������������ÿ5msִ��һ��
	}
}

/*
	�������ڽ����ж�// USART3 ->USART1
*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{ 
		if(Uart1Index>=Uart1BufferSize)Uart1Index = 0;
		Uart1Buffer[Uart1Index++]=USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

