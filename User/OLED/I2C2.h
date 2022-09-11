/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	I2C.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/
#ifndef _I2C2_H
#define _I2C2_H


#include "stm32f10x.h"


#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define true 1
#define false 0 
#define   bool  uint8_t

#define TRUE  0
#define FALSE -1
/*
#define MPU6050_READRATE			1000	//6050读取频率
#define MPU6050_READTIME			0.001	//6050读取时间间隔
#define EE_6050_ACC_X_OFFSET_ADDR	0
#define EE_6050_ACC_Y_OFFSET_ADDR	1
#define EE_6050_ACC_Z_OFFSET_ADDR	2
#define EE_6050_GYRO_X_OFFSET_ADDR	3
#define EE_6050_GYRO_Y_OFFSET_ADDR	4
#define EE_6050_GYRO_Z_OFFSET_ADDR	5
*/
////0表示写
//#define	I2C_Direction_Transmitter   0
////１表示读
//#define	I2C_Direction_Receiver      1	 
/*====================================================================================================*/
/*====================================================================================================*/
bool i2c2WriteBuffer(uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
bool i2c2Write(uint8_t addr_, uint8_t reg_, uint8_t data);
bool i2c2Read(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf);
void i2c2Init(void);
static void i2c2Unstick(void);

int8_t i2c2write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);
int8_t i2c2read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
/*====================================================================================================*/
/*====================================================================================================*/
#endif

