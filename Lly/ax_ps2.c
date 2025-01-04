/*
******************************************************************************
  * @˵  ��
  *
  *   PS2���ݶ���
  *   BYTE   DATA   ����
  *   01     idle
  *   02     0x73   �ֱ�����ģʽ
  *   03     0x5A   Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7
  *   04     data   SLCT  JOYR  JOYL  STRT   UP   RGIHT  DOWN   L
  *   05     data   L2     R2     L1    R1   /\     O     X    ��
  *   06     data   �ұ�ҡ��  0x00 = ��    0xff = ��
  *   07     data   �ұ�ҡ��  0x00 = ��    0xff = ��
  *   08     data   ���ҡ��  0x00 = ��    0xff = ��
  *   09     data   ���ҡ��  0x00 = ��    0xff = ��
  * 
  ******************************************************************************
  */

#include <stdint-gcc.h>
#include <stdio.h>
#include "ax_ps2.h"
#include "stm32h7xx.h"


//PS2�ֱ������������
#define DI()       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)   //������������

#define CMD_H()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1)  //��8��λ��
#define CMD_L()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0)  //����λ��


#define CS_H()     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1)
#define CS_L()     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0)

#define CLK_H()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1)  //ʱ������
#define CLK_L()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0)  //ʱ������

JOYSTICK_TypeDef my_joystick={};  //�ֱ���ֵ�ṹ��


const  uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   //�����ȡ��������                         
static uint8_t PS2_data[9] = {0};  //���յ�����
void delay_us(__IO uint32_t delay);

/**
  * @��  ��  PS2���ݶ�д����
  * @��  ��  data:Ҫд�������
  * @����ֵ  ��������
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

	//���ض�������
    return res;	
}

/**
  * @��  ��  PS2��ȡ������ҡ����ֵ��
  * @��  ��  *JoystickStruct �ֱ���ֵ�ṹ��
  * @����ֵ  ��
  */
void AX_PS2_ScanKey(JOYSTICK_TypeDef *JoystickStruct)
{
	uint8_t i;
	
	//ʹ���ֱ�
	CS_L();
	
	//��ȡPS2����
	for(i=0; i<9; i++)
	{
		  PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
          delay_us(16);
	}
	
	//�ر�ʹ��
	CS_H();

	//��ֵ����
	JoystickStruct->mode = PS2_data[1];
	JoystickStruct->btn1 = ~PS2_data[3];
	JoystickStruct->btn2 = ~PS2_data[4];
	JoystickStruct->RJoy_LR = PS2_data[5];
	JoystickStruct->RJoy_UD = PS2_data[6];
	JoystickStruct->LJoy_LR = PS2_data[7];
	JoystickStruct->LJoy_UD = PS2_data[8];

}


/**
  * @��  ��  ���΢����ʱ
  * @��  ��  us����ʱ���ȣ���λus
  * @����ֵ  ��
  */
#define CPU_FREQUENCY_MHZ    480		///ʱ����Ƶ ��ʱʱ�����Ҫ�ʼ����һֱ������޷���������
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

