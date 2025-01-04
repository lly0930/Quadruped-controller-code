//
// Created by 29787 on 2024/10/25.
//

#ifndef PROJECT_IK_FOOTTRAJECTORY_H
#define PROJECT_IK_FOOTTRAJECTORY_H

#define length_leg_big 0.200001f
#define length_leg_big_2 0.04000000f

#define length_leg_small 0.200001f
#define length_leg_small_2 0.04000000f
typedef struct {
    float  Posi_lepro_big;
    float  Posi_ripro_big;
    float  Posi_lebe_big;
    float  Posi_ribe_big;
    float  Posi_lepro_small;
    float  Posi_ripro_small;
    float  Posi_lebe_small;
    float  Posi_ribe_small;
    float  x_rfmotor;
    float  x_rbmotor;
    float  x_lfmotor;
    float  x_lbmotor;
    float  y_rfmotor;
    float  y_rbmotor;
    float  y_lfmotor;
    float  y_lbmotor;
}motor_posi;
extern motor_posi Motor_posi;


void motor_IK();
void foot_particlePath_assign(float faai_);
void Sway_Support_foot_particlePath(float t, float h_z, float h_b, float xs, float xf, float* x_b,float* y_b,float* x_z,float* y_z);
#endif //PROJECT_IK_FOOTTRAJECTORY_H
