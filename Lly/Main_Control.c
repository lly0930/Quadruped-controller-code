//
// Created by 29787 on 2024/10/25.
//
#include <stdbool.h>
#include "Main_Control.h"
#include "Serial.h"
#include "ax_ps2.h"
#include "bsp_can.h"
#include "motor_control.h"
#include "IK_FooTtrajectory.h"

extern float t;
extern float faai;
extern float cb_xz,cb_yz,cb_xb,cb_yb;
extern float x_s,x_f;
extern float h__b,h__z;
float xep_z=0,xep_b=0,zep=0;

#define relax_state 0
#define getdown_state 1
#define halfsquat_state 2
#define forward_state 3
#define initial_state 4
#define retreat_state 5

uint8_t dog_state=initial_state;

//狗腿坐标系  正看lb腿的视角      狗腿电机都是顺时针为正
//----------------------->  x
//|
//|
//|
//|
//|
//↓  y

_Noreturn void Main_()
{
    Serial_idle_recei_init();
    FDCAN1_RX_Filter_Init();
    Motor_posi.x_rfmotor=0.0f;
    Motor_posi.y_rfmotor=0.4f;
    Motor_posi.x_rbmotor=0.0f;
    Motor_posi.y_rbmotor=0.4f;
    Motor_posi.x_lfmotor=0.0f;
    Motor_posi.y_lfmotor=0.4f;
    Motor_posi.x_lbmotor=0.0f;
    Motor_posi.y_lbmotor=0.4f;
    while (true)
    {


        AX_PS2_ScanKey(&my_joystick);
//        printf("MODE:%2x BTN1:%2x BTN2:%2x RJOY_LR:%2x RJOY_UD:%2x LJOY_LR:%2x LJOY_UD:%2x\r\n",
//               my_joystick.mode, my_joystick.btn1, my_joystick.btn2,
//               my_joystick.RJoy_LR, my_joystick.RJoy_UD, my_joystick.LJoy_LR, my_joystick.LJoy_UD);
        if(my_joystick.btn2==1)
        {
            my_joystick.btn2=0;
            dog_state=getdown_state;
        }

        if(my_joystick.btn2==4)
        {
            my_joystick.btn2=0;
            dog_state=halfsquat_state;
        }

        if(my_joystick.btn2==8)
        {
            my_joystick.btn2=0;
            dog_state=forward_state;
        }

        if(my_joystick.btn2==2)
        {
            my_joystick.btn2=0;
            dog_state=retreat_state;
        }

        switch (dog_state) {
            case getdown_state:
            {
                Motor_posi.x_rfmotor=0.0f;
                Motor_posi.y_rfmotor=0.15f;
                Motor_posi.x_rbmotor=0.0f;
                Motor_posi.y_rbmotor=0.15f;
                Motor_posi.x_lfmotor=0.0f;
                Motor_posi.y_lfmotor=0.15f;
                Motor_posi.x_lbmotor=0.0f;
                Motor_posi.y_lbmotor=0.15f;
            }
                break;

            case halfsquat_state:
            {
                Motor_posi.x_rfmotor=0.0f;
                Motor_posi.y_rfmotor=0.3f;
                Motor_posi.x_rbmotor=0.0f;
                Motor_posi.y_rbmotor=0.3f;
                Motor_posi.x_lfmotor=0.0f;
                Motor_posi.y_lfmotor=0.3f;
                Motor_posi.x_lbmotor=0.0f;
                Motor_posi.y_lbmotor=0.3f;
            }
                break;

            case forward_state:
            {
                x_s=-0.08f,x_f=0.08f;
                t+=0.01f;
                if(t>=1)
                    t=0;
                Sway_Support_foot_particlePath(t,h__z,h__b,x_s,x_f,&cb_xb,&cb_yb,&cb_xz,&cb_yz);
                foot_particlePath_assign(faai);
            }
                break;

            case retreat_state:
            {
                x_s=0.08f,x_f=-0.08f;
                t+=0.01f;
                if(t>=1)
                    t=0;
                Sway_Support_foot_particlePath(t,h__z,h__b,x_s,x_f,&cb_xb,&cb_yb,&cb_xz,&cb_yz);
                foot_particlePath_assign(faai);
            }
                break;
        }
        motor_IK();
        motor_set_position();
        HAL_Delay(20);
        HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
        Vofa_date.vofa_trans[0]=Motor_posi.Posi_ripro_big;
        Vofa_date.vofa_trans[1]=Motor_posi.Posi_ripro_small;
        Vofa_date.vofa_trans[2]=Motor_posi.Posi_ribe_big;
        Vofa_date.vofa_trans[3]=Motor_posi.Posi_ribe_small;
        Vofa_date.vofa_trans[4]=Motor_posi.Posi_lepro_big;
        Vofa_date.vofa_trans[5]=Motor_posi.Posi_lepro_small;
        Vofa_date.vofa_trans[6]=Motor_posi.Posi_lebe_big;
        Vofa_date.vofa_trans[7]=Motor_posi.Posi_lebe_small;
        Vofa_date.vofa_trans[8]=dog_state;
       Vofa_transmit();

    }
}


