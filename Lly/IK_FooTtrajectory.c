//
// Created by 29787 on 2024/10/25.
//

#include "IK_FooTtrajectory.h"
#include "main.h"
#define PI 3.1415926
motor_posi Motor_posi={};

void Inverse_Kinematics(float x,float y, float* motor_big, float* motor_small)  //y在过程中取了绝对值，也就是足端只会在身子下方
{
    *motor_small=180-acosf((x*x + y*y -length_leg_big_2 - length_leg_small_2) / ((length_leg_small*length_leg_big)*(-2)))*57.2957795-0;
    if(x<0)
        *motor_big=atanf(y/x)*57.2957795- acos((x*x + y*y + length_leg_big_2 - length_leg_small_2 ) / ( sqrt(x*x + y*y)*2*length_leg_big))*57.2957795-90+180;
    else
        *motor_big=atanf(y/x)*57.2957795- acos((x*x + y*y + length_leg_big_2 - length_leg_small_2 ) / ( sqrt(x*x + y*y)*2*length_leg_big))*57.2957795-90;

}
void motor_IK()
{
    Inverse_Kinematics(Motor_posi.x_rfmotor,Motor_posi.y_rfmotor, &Motor_posi.Posi_ripro_big, &Motor_posi.Posi_ripro_small);
    Inverse_Kinematics(Motor_posi.x_rbmotor,Motor_posi.y_rbmotor, &Motor_posi.Posi_ribe_big , &Motor_posi.Posi_ribe_small);
    Inverse_Kinematics(Motor_posi.x_lfmotor,Motor_posi.y_lfmotor, &Motor_posi.Posi_lepro_big, &Motor_posi.Posi_lepro_small);
    Inverse_Kinematics(Motor_posi.x_lbmotor,Motor_posi.y_lbmotor, &Motor_posi.Posi_lebe_big,  &Motor_posi.Posi_lebe_small);
}
#define pi 3.1415926
float rf1;
float rb1;
float lf1;
float lb1;
float rf2;
float rb2;
float lf2;
float lb2;

float t=0;
float faai=0.5f;
float cb_xz,cb_yz,cb_xb,cb_yb;
float x_s=-0.08f,x_f=0.08f;
float h__b=0.03f,h__z=0.3f;

void Sway_Support_foot_particlePath(float t, float h_z, float h_b, float xs, float xf, float* x_b,float* y_b,float* x_z,float* y_z)
{
    float trans_t=t;
    if(t>faai)
    {
        trans_t=trans_t-faai;
    }

    float sigma=2*pi*trans_t/(faai);
    float zep=h_b*(1-cos(sigma))/2;
    float xep_z=(xs-xf)*((sigma-sin(sigma))/(2*pi))+xf;
    float xep_b=(xf-xs)*((sigma-sin(sigma))/(2*pi))+xs;

    *x_b=-xep_b;
    *y_b=h_z-zep;

    *x_z=-xep_z;
    *y_z=h_z;
}

void foot_particlePath_assign(float faai_)
{
    if(t<faai_)
    {

        Motor_posi.x_rfmotor=cb_xb;
        Motor_posi.x_lbmotor=cb_xb;
        Motor_posi.x_rbmotor=cb_xz;
        Motor_posi.x_lfmotor=cb_xz;

        Motor_posi.y_rfmotor=cb_yb;
        Motor_posi.y_lbmotor=cb_yb;
        Motor_posi.y_rbmotor=cb_yz;
        Motor_posi.y_lfmotor=cb_yz;
    }
    if(t>faai_)
    {
        Motor_posi.x_rfmotor=cb_xz;
        Motor_posi.x_lbmotor=cb_xz;
        Motor_posi.x_rbmotor=cb_xb;
        Motor_posi.x_lfmotor=cb_xb;

        Motor_posi.y_rfmotor=cb_yz;
        Motor_posi.y_lbmotor=cb_yz;
        Motor_posi.y_rbmotor=cb_yb;
        Motor_posi.y_lfmotor=cb_yb;
    }
}