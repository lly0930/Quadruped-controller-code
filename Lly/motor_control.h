//
// Created by 29787 on 2024/10/25.
//

#ifndef PROJECT_MOTOR_CONTROL_H
#define PROJECT_MOTOR_CONTROL_H

#include "stdint.h"

#define posi_trans 1
#define lb_big 0x01
#define lb_sma 0x02

#define rb_big 0x03
#define rb_sma 0x04

#define rf_big 0x05
#define rf_sma 0x06

#define lf_big 0x07
#define lf_sma 0x08

void Motor_sendDate();
void motor_set_position();

#endif //PROJECT_MOTOR_CONTROL_H
