/**********************************************************************
Author:     1+1>2 Team
技术论坛：
购买链接：
File:  	mpu6050.h
@Copyright：by 1+1>2
Version：V1.0
Date：2020
@brief：2020 	




***********************************************************************/
#ifndef __MPU6050_H__
#define __MPU6050_H__

extern short g_fGyro_x,g_fAccel_y,g_fAccel_z;

void MPU6050_Init(void);
void MPU6050_Pose(void);


#endif

