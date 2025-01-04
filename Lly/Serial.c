//
// Created by lly on 2024/8/9.
//

#include "Serial.h"

/**
  ******************************************************************************
  * @author         : lly
  * @attention      : �����ض���  printf�������
  *                 ��.h�ļ���  ��ע��  #define Serial_redefine 1  ����ѡ�񴮿ںż���
  ******************************************************************************
  */
#if Serial_redefine
#include "usart.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&Serial_redifine_port, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
#endif


/**
  ******************************************************************************
  * @author         : lly
  * @attention      : ���ڵ�idle�жϽ���
  *                   ��.h�ļ���  ��ע��  #define Serial_dmaIdle_receive 1  ����ѡ�񴮿ںż���
  *                   ��һ������Ϊ��ʼ������  ���ڶ�������Ϊ�жϴ��������Ž����жϼ���   hander�м���
  *                   ���ܵ�������Seri_Idle.recbuff������
  ******************************************************************************
  */
#if Serial_dmaIdle_receive
Str_Seri_Idle_ Seri_Idle={};
void Serial_idle_recei_init()
{
    HAL_UART_Receive_DMA(&Serial_dmaIdle_receive_port,Seri_Idle.recbuff,100);      //ʹ���µĽ���
    __HAL_UART_ENABLE_IT(&Serial_dmaIdle_receive_port,UART_IT_IDLE);  //���������ж�        д���������У�ѭ���⼴��
}

void Serial_idle_cb()
{
    if (__HAL_UART_GET_FLAG(&Serial_dmaIdle_receive_port, UART_FLAG_IDLE) != RESET)  //��������˿����ж�
    {
        __HAL_UART_CLEAR_IDLEFLAG(&Serial_dmaIdle_receive_port);         //����жϱ�־λ
        HAL_UART_DMAStop(&Serial_dmaIdle_receive_port);            //ֹͣ����
        uint8_t len = 100 - __HAL_DMA_GET_COUNTER(Serial_dmaIdle_receive_port.hdmarx);  //������յ���λ������������ʾ���ǻ��м����ֽ�δ���յ�
        HAL_UART_Transmit_DMA(&Serial_dmaIdle_receive_port, Seri_Idle.recbuff, len);  //���ս��յ�λ������
        HAL_UART_Receive_DMA(&Serial_dmaIdle_receive_port, Seri_Idle.recbuff, 100);  //ʹ���µĽ���
    }
}
#endif

/**
  ******************************************************************************
  * @author         : lly
  * @attention      : Vofa justfloat��ʾ   ���Ͷ�Ϊfloat����  ֱ�Ӹ�ǰ��λ���鸳ֵ����
  *                   ��.h�ļ���  ��ע��  #define Vofa_trans 1  ѡ��usb������ͨ���� ��ͨ����Ҫѡ�񴮿ں� ע�����ݴ�С����Ҫ����date_num
  ******************************************************************************
  *
  *
  */
#if Vofa_trans
#include <string.h>
#if(Vofa_trans_mode==Vofa_trans_by_usb)
    #include "usbd_cdc_if.h"
#endif
Vofa_date_ Vofa_date={};
void Vofa_transmit()
{
    memcpy(Vofa_date.vofa_trans_,(uint8_t*)Vofa_date.vofa_trans,sizeof(Vofa_date.vofa_trans));
    Vofa_date.vofa_trans_[date_num*4] = 0X00;
    Vofa_date.vofa_trans_[date_num*4+1] = 0X00;
    Vofa_date.vofa_trans_[date_num*4+2] = 0X80;
    Vofa_date.vofa_trans_[date_num*4+3] = 0X7F;
#if(Vofa_trans_mode==Vofa_trans_by_usb)
        CDC_Transmit_FS((uint8_t*)(Vofa_date.vofa_trans_), 68);
#endif
#if(Vofa_trans_mode==Vofa_trans_by_serial)
        HAL_UART_Transmit(&Serial_dmaIdle_receive_port,(uint8_t*)(Vofa_date.vofa_trans_), date_num*4+4,0xffff);  //���ս��յ�λ������
#endif
}
#endif