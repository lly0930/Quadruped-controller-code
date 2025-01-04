/*
******************************************************************************
  * @说  明
  *
  *   PS2数据定义
  *   BYTE   DATA   解释
  *   01     idle
  *   02     0x73   手柄工作模式
  *   03     0x5A   Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7
  *   04     data   SLCT  JOYR  JOYL  STRT   UP   RGIHT  DOWN   L
  *   05     data   L2     R2     L1    R1   /\     O     X    口
  *   06     data   右边摇杆  0x00 = 左    0xff = 右
  *   07     data   右边摇杆  0x00 = 上    0xff = 下
  *   08     data   左边摇杆  0x00 = 左    0xff = 右
  *   09     data   左边摇杆  0x00 = 上    0xff = 下
  * 
  ******************************************************************************
  */

#include <stdint-gcc.h>
#include <stdio.h>
#include "ax_ps2.h"
#include "stm32h7xx.h"


//PS2手柄的输入输出口
#define DI()       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)   //数据输入引脚

#define CMD_H()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1)  //命8令位高
#define CMD_L()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0)  //命令位低


#define CS_H()     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1)
#define CS_L()     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0)

#define CLK_H()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1)  //时钟拉高
#define CLK_L()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0)  //时钟拉低

JOYSTICK_TypeDef my_joystick={};  //手柄键值结构体


const  uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   //请求获取数据命令                         
static uint8_t PS2_data[9] = {0};  //接收的数据
void delay_us(__IO uint32_t delay);

/**
  * @简  述  PS2数据读写函数
  * @参  数  data:要写入的数据
  * @返回值  读出数据
  */
static uint8_t PS2_ReadWriteData(uint8_t data)
{
	uint8_t ref, res = 0;
	
	
	for(ref = 0x01; ref > 0x00; ref <<= 1)
	{
		CLK_L();
		if(ref&data)
			CMD_H();
		else
			CMD_L();

		delay_us(16);
		
		CLK_H();
		if(DI())
			res |= ref;
		delay_us(16);
	}
	
	CMD_H();

	//返回读出数据
    return res;	
}

/**
  * @简  述  PS2获取按键及摇杆数值。
  * @参  数  *JoystickStruct 手柄键值结构体
  * @返回值  无
  */
void AX_PS2_ScanKey(JOYSTICK_TypeDef *JoystickStruct)
{
	uint8_t i;
	
	//使能手柄
	CS_L();
	
	//读取PS2数据
	for(i=0; i<9; i++)
	{
		  PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
          delay_us(16);
	}
	
	//关闭使能
	CS_H();

	//数值传递
	JoystickStruct->mode = PS2_data[1];
	JoystickStruct->btn1 = ~PS2_data[3];
	JoystickStruct->btn2 = ~PS2_data[4];
	JoystickStruct->RJoy_LR = PS2_data[5];
	JoystickStruct->RJoy_UD = PS2_data[6];
	JoystickStruct->LJoy_LR = PS2_data[7];
	JoystickStruct->LJoy_UD = PS2_data[8];

}


/**
  * @简  述  软件微妙延时
  * @参  数  us：延时长度，单位us
  * @返回值  无
  */
#define CPU_FREQUENCY_MHZ    480		///时钟主频 延时时间很重要最开始就是一直错误而无法读到数据
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

