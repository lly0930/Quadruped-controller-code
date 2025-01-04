//
// Created by 29787 on 2024/10/25.
//
#include "IK_FooTtrajectory.h"
#include "motor_control.h"
#include "bsp_can.h"
#include "Serial.h"
uint8_t can_send_data[8]={0};
void send_position(uint8_t id, int16_t tar_pos)
{
    if(id>4){
        can_send_data[(id - 5)*2]   = tar_pos >> 8;
        can_send_data[(id - 5)*2+1] = tar_pos;
    }
    else{
        can_send_data[(id - 1)*2]   = tar_pos >> 8;
        can_send_data[(id - 1)*2+1] = tar_pos;
    }

    FDCAN1_Send_Msg(id,can_send_data,8);
}

void motor_set_position()
{
    int16_t lbbpos=(int16_t)(Motor_posi.Posi_lebe_big*10*8/360.f);     //这两个电机正方向相对于其他电机是反的
    int16_t lbspos=(int16_t)(-Motor_posi.Posi_lebe_small*10*8/360.f);
    int16_t rbbpos=(int16_t)(-Motor_posi.Posi_ribe_big*10*8/360.f);
    int16_t rbspos=(int16_t)(Motor_posi.Posi_ribe_small*10*8/360.f);
    int16_t lfbpos=(int16_t)(-Motor_posi.Posi_lepro_big*10*8/360.f);
    int16_t lfspos=(int16_t)(Motor_posi.Posi_lepro_small*10*8/360.f);
    int16_t rfbpos=(int16_t)(-Motor_posi.Posi_ripro_big*10*8/360.f);
    int16_t rfspos=(int16_t)(Motor_posi.Posi_ripro_small*10*8/360.f);
//    Vofa_date.vofa_trans[8]=lbbpos;
//    Vofa_date.vofa_trans[9]=lbspos;
//    Vofa_date.vofa_trans[10]=rbbpos;
//    Vofa_date.vofa_trans[11]=rbspos;
//    Vofa_date.vofa_trans[12]=lfbpos;
//    Vofa_date.vofa_trans[13]=lfspos;
//    Vofa_date.vofa_trans[14]=rfbpos;
//    Vofa_date.vofa_trans[15]=rfspos;
    send_position(lb_big,lbbpos);
    HAL_Delay(1);
    send_position(lb_sma,lbspos);
    HAL_Delay(1);
    send_position(lf_big,lfbpos);
    HAL_Delay(1);
    send_position(lf_sma,lfspos);
    HAL_Delay(1);
    send_position(rb_big,rbbpos);
    HAL_Delay(1);
    send_position(rb_sma,rbspos);
    HAL_Delay(1);
    send_position(rf_big,rfbpos);
    HAL_Delay(1);
    send_position(rf_sma,rfspos);
    HAL_Delay(1);
}
