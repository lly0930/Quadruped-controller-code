//
// Created by lly on 2024/8/9.
//
#include "stm32h7xx.h"

#ifndef H750_LLY_SERIAL_H
#define H750_LLY_SERIAL_H

///////////////////////////////////ºêÐÞ¸ÄÇø////////////////////////////////////
#define Serial_redefine 1
#define Serial_redifine_port huart1

#define Serial_dmaIdle_receive 1
#define Serial_dmaIdle_receive_port huart1

#define Vofa_trans 1
#define date_num   16
#define Vofa_trans_by_usb 1
#define Vofa_trans_by_serial 2
#define Vofa_trans_mode Vofa_trans_by_serial
/////////////////////////////////////////////////////////////////////////////


#if Vofa_trans
typedef struct {
    float   vofa_trans[date_num];
    uint8_t vofa_trans_[date_num*4+4];
}Vofa_date_;
extern Vofa_date_ Vofa_date;
void Vofa_transmit();
#endif


#if Serial_redefine
#include "stdio.h"
#endif

#if Serial_dmaIdle_receive
typedef struct{
    uint8_t recbuff[100];
}Str_Seri_Idle_;
extern Str_Seri_Idle_ Seri_Idle;
void Serial_idle_recei_init();
void Serial_idle_cb();
#endif



#endif //H750_LLY_SERIAL_H
