/**********************************************************************
版权所有：	喵呜创新科技，2017.
官		网：	http://www.miaowlabs.com
淘		宝：	https://shop275516297.taobao.com/
文 件 名: 	I2C.c
作    者:   喵呜实验室
版		本:   3.00
完成日期:   2017.03.01
概		要: 	


***********************************************************************/

#include "I2C2.h"


#define SCL_H         GPIOA->BSRR = GPIO_Pin_0 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOA->BRR  = GPIO_Pin_0 /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOA->BSRR = GPIO_Pin_1 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOA->BRR  = GPIO_Pin_1 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read      GPIOA->IDR  & GPIO_Pin_0 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIOA->IDR  & GPIO_Pin_1 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */


static void I2C2_delay(void)
{
    volatile int i = 7;
    while (i)
        i--;
}

static bool I2C2_Start(void)
{
    SDA_H;
    SCL_H;
    I2C2_delay();
    if (!SDA_read)
        return false;
    SDA_L;
    I2C2_delay();
    if (SDA_read)
        return false;
    SDA_L;
    I2C2_delay();
    return true;
}

static void I2C2_Stop(void)
{
    SCL_L;
    I2C2_delay();
    SDA_L;
    I2C2_delay();
    SCL_H;
    I2C2_delay();
    SDA_H;
    I2C2_delay();
}

static void I2C2_Ack(void)
{
    SCL_L;
    I2C2_delay();
    SDA_L;
    I2C2_delay();
    SCL_H;
    I2C2_delay();
    SCL_L;
    I2C2_delay();
}

static void I2C2_NoAck(void)
{
    SCL_L;
    I2C2_delay();
    SDA_H;
    I2C2_delay();
    SCL_H;
    I2C2_delay();
    SCL_L;
    I2C2_delay();
}

static bool I2C2_WaitAck(void)
{
    SCL_L;
    I2C2_delay();
    SDA_H;
    I2C2_delay();
    SCL_H;
    I2C2_delay();
    if (SDA_read) {
        SCL_L;
        return false;
    }
    SCL_L;
    return true;
}

static void I2C2_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--) {
        SCL_L;
        I2C2_delay();
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        I2C2_delay();
        SCL_H;
        I2C2_delay();
    }
    SCL_L;
}

static uint8_t I2C2_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    SDA_H;
    while (i--) {
        byte <<= 1;
        SCL_L;
        I2C2_delay();
        SCL_H;
        I2C2_delay();
        if (SDA_read) {
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}

void i2c2Init(void)
{
 	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);    

	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //KKDD// 8,9 ->10,11
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA, &gpio);

}

bool i2c2Write(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (!I2C2_Start())
        return false;
    I2C2_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C2_WaitAck()) {
        I2C2_Stop();
        return false;
    }
    I2C2_SendByte(reg);
    I2C2_WaitAck();
    I2C2_SendByte(data);
    I2C2_WaitAck();
    I2C2_Stop();
    return true;
}

bool i2c2WriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    if (!I2C2_Start())
        return false;
    I2C2_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C2_WaitAck()) {
        I2C2_Stop();
        return false;
    }
    I2C2_SendByte(reg);
    I2C2_WaitAck();
    for (i = 0; i < len; i++) {
        I2C2_SendByte(data[i]);
        if (!I2C2_WaitAck()) {
            I2C2_Stop();
            return false;
        }
    }
    I2C2_Stop();
    return true;
}


bool i2c2Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!I2C2_Start())
        return false;
    I2C2_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C2_WaitAck()) {
        I2C2_Stop();
        return false;
    }
    I2C2_SendByte(reg);
    I2C2_WaitAck();
    I2C2_Start();
    I2C2_SendByte(addr << 1 | I2C_Direction_Receiver);
    I2C2_WaitAck();
    while (len) {
        *buf = I2C2_ReceiveByte();
        if (len == 1)
            I2C2_NoAck();
        else
            I2C2_Ack();
        buf++;
        len--;
    }
    I2C2_Stop();
    return true;
}

int8_t i2c2write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	if(i2c2WriteBuffer(addr,reg,len,data))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
int8_t i2c2read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(i2c2Read(addr,reg,len,buf))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

