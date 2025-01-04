//
// Created by lly on 2024/8/9.
//

#include "Serial.h"

/**
  ******************************************************************************
  * @author         : lly
  * @attention      : 串口重定向  printf输出数据
  *                 在.h文件中  解注释  #define Serial_redefine 1  并且选择串口号即可
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
  * @attention      : 串口的idle中断接收
  *                   在.h文件中  解注释  #define Serial_dmaIdle_receive 1  并且选择串口号即可
  *                   第一个函数为初始化接收  ，第二个函数为中断处理函数，放接收中断即可   hander中即可
  *                   接受的数据在Seri_Idle.recbuff数组中
  ******************************************************************************
  */
#if Serial_dmaIdle_receive
Str_Seri_Idle_ Seri_Idle={};
void Serial_idle_recei_init()
{
    HAL_UART_Receive_DMA(&Serial_dmaIdle_receive_port,Seri_Idle.recbuff,100);      //使能新的接收
    __HAL_UART_ENABLE_IT(&Serial_dmaIdle_receive_port,UART_IT_IDLE);  //开启空闲中断        写在主函数中，循环外即可
}

void Serial_idle_cb()
{
    if (__HAL_UART_GET_FLAG(&Serial_dmaIdle_receive_port, UART_FLAG_IDLE) != RESET)  //如果进入了空闲中断
    {
        __HAL_UART_CLEAR_IDLEFLAG(&Serial_dmaIdle_receive_port);         //清除中断标志位
        HAL_UART_DMAStop(&Serial_dmaIdle_receive_port);            //停止接收
        uint8_t len = 100 - __HAL_DMA_GET_COUNTER(Serial_dmaIdle_receive_port.hdmarx);  //计算接收到的位数，参数三表示的是还有几个字节未接收到
        HAL_UART_Transmit_DMA(&Serial_dmaIdle_receive_port, Seri_Idle.recbuff, len);  //按照接收的位数发送
        HAL_UART_Receive_DMA(&Serial_dmaIdle_receive_port, Seri_Idle.recbuff, 100);  //使能新的接收
    }
}
#endif

/**
  ******************************************************************************
  * @author         : lly
  * @attention      : Vofa justfloat显示   发送都为float类型  直接给前几位数组赋值即可
  *                   在.h文件中  解注释  #define Vofa_trans 1  选择usb还是普通串口 普通串口要选择串口号 注意数据大小，不要超出date_num
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
        HAL_UART_Transmit(&Serial_dmaIdle_receive_port,(uint8_t*)(Vofa_date.vofa_trans_), date_num*4+4,0xffff);  //按照接收的位数发送
#endif
}
#endif