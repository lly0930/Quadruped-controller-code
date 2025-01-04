/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_PS2_H
#define __AX_PS2_H

/* Includes ------------------------------------------------------------------*/	 
#include "stm32h7xx.h"

//PS2�ֱ���ֵ���ݽṹ��	 
typedef struct			 				
{
  uint8_t mode;		    /* �ֱ��Ĺ���ģʽ */

  uint8_t btn1;         /* B0:SLCT B3:STRT B4:UP B5:R B6:DOWN B7:L   */

  uint8_t btn2;         /* B0:L2 B1:R2 B2:L1 B3:R1 B4:/\ B5:O B6:X B7:�� */

  uint8_t RJoy_LR;      /* �ұ�ҡ��  0x00 = ��    0xff = ��   */

  uint8_t RJoy_UD;      /* �ұ�ҡ��  0x00 = ��    0xff = ��   */

  uint8_t LJoy_LR;      /* ���ҡ��  0x00 = ��    0xff = ��   */

  uint8_t LJoy_UD;      /* ���ҡ��  0x00 = ��    0xff = ��   */
	
}JOYSTICK_TypeDef;


/*** PS2�����ֱ��������� **********/
void AX_PS2_ScanKey(JOYSTICK_TypeDef* JoystickStruct);//PS2��ȡ������ҡ����ֵ
extern JOYSTICK_TypeDef my_joystick;  //�ֱ���ֵ�ṹ��

#endif 

/******************* (C) ��Ȩ 2023 XTARK **************************************/
