#include "common.h"	 
#include "usart.h" 			 
//#include "usart2.h" 			 
#include "hc05.h" 
//#include "led.h" 
#include "string.h"	 
#include "math.h"
#include "communicate.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-HC05蓝牙模块驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20140329
//修改LED的连接，原来接PC5，改为PA4，以兼容MiniSTM32开发板V3.0									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化ATK-HC05模块
//返回值:0,成功;1,失败.
u8 HC05_Init(void)
{
	u8 retry=10,t;	  		 
	u8 temp=1;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	//使能PORTA C时钟	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化A4
	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC4

	GPIO_SetBits(GPIOA,GPIO_Pin_1);
 	
	//USART1Init(9600);	//初始化串口2为:9600,波特率.
	
	while(retry--)
	{
		HC05_KEY1;					//KEY置高,进入AT模式
		delay_ms(50);
		//Uart1SendStr("AT\r\n");		//发送AT测试指令
		Uart1SendStr(" \r\nAT+NAME=HC05 Master by 1+1>2 team\r\n");//长时间按住蓝牙模块key，上电，等待10秒钟后蓝牙即可配置成功
		delay_ms(50);
		HC05_KEY0;					//KEY拉低,退出AT模式
		for(t=0;t<10;t++) 			//最长等待50ms,来接收HC05模块的回应
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
		HC05_KEY0;					//KEY拉低,退出AT模式
		for(t=0;t<10;t++) 			//最长等待50ms,来接收HC05模块的回应
		{
			if(ParseAT(Uart1Buffer))
			{
				temp = 0;
				break;
			}
				delay_ms(5);
		}	
		
//		if(USART2_RX_STA&0X8000)	//接收到一次数据了
//		{
//			temp=USART2_RX_STA&0X7FFF;	//得到数据长度
//			USART2_RX_STA=0;			 
//			if(temp==4&&USART2_RX_BUF[0]=='O'&&USART2_RX_BUF[1]=='K')
//			{
//				temp=0;//接收到OK响应
//				break;
//			}
//		}			    		
	}		    
	if(retry==0)temp=1;	//检测失败
	return temp;	 
}	 
//获取ATK-HC05模块的角色
//返回值:0,从机;1,主机;0XFF,获取失败.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY1;					//KEY置高,进入AT模式
		delay_ms(10);
		Uart1SendStr("AT+ROLE?\r\n");	//查询角色
		for(t=0;t<20;t++) 			//最长等待200ms,来接收HC05模块的回应
		{
			delay_ms(10);
			if(ParseAT(Uart1Buffer))break;
		}		
		HC05_KEY0;					//KEY拉低,退出AT模式
		if(ParseAT(Uart1Buffer))	//接收到一次数据了
		{
//			temp=USART2_RX_STA&0X7FFF;	//得到数据长度
//			USART2_RX_STA=0;			 
//			if(temp==13&&USART2_RX_BUF[0]=='+')//接收到正确的应答了
//			{
				temp=Uart1Buffer[6]-'0';//得到主从模式值
				break;
//			}
		}		
	}
	if(retry==0)temp=0XFF;//查询失败.
	return temp;
} 							   
//ATK-HC05设置命令
//此函数用于设置ATK-HC05,适用于仅返回OK应答的AT指令
//atstr:AT指令串.比如:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"等字符串
//返回值:0,设置成功;其他,设置失败.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY1;					//KEY置高,进入AT模式
		delay_ms(10);
		Uart1SendStr((char*)atstr);	//发送AT字符串
		HC05_KEY0;					//KEY拉低,退出AT模式
		for(t=0;t<20;t++) 			//最长等待100ms,来接收HC05模块的回应
		{
			if(ParseAT(Uart1Buffer))break;
			delay_ms(5);
		}		
		if(ParseAT(Uart1Buffer))	//接收到一次数据了
			{		
				temp=0;
				break;			 
			}		
	}
	if(retry==0)temp=0XFF;//设置失败.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//通过该函数,可以利用USMART,调试接在串口2上的ATK-HC05模块
//str:命令串.(这里注意不再需要再输入回车符)
void HC05_CFG_CMD(u8 *str)
{					  
//	u8 temp;
	u8 t;		  
	HC05_KEY1;						//KEY置高,进入AT模式
	delay_ms(10);
	Uart1SendStr((char*)str); //发送指令
	for(t=0;t<50;t++) 				//最长等待500ms,来接收HC05模块的回应
	{
		if(ParseAT(Uart1Buffer))break;
		delay_ms(10);
	}									    
	HC05_KEY0;						//KEY拉低,退出AT模式
//	if(USART2_RX_STA&0X8000)		//接收到一次数据了
//	{
//		temp=USART2_RX_STA&0X7FFF;	//得到数据长度
//		USART2_RX_STA=0;
//		USART2_RX_BUF[temp]=0;		//加结束符		 
//		printf("\r\n%s",USART2_RX_BUF);//发送回应数据到串口1
//	} 				 
}














