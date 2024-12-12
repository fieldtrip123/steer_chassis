//
// Created by Administrator on 2024/12/3.
//

#ifndef __CHASSIS__H
#define __CHASSIS__H

#include "main.h"
#include "mycan.h"
#define PI       3.14159265358979323846f
#define RAD2DEG  57.295779513f
#define DEG2ENC  22.752777777f
#define  M3508_REDUCTION_RATIO  (9.0f)
#define  RADIUS  1
void set_chassis_3508_speed(int16_t v1,int16_t v2,int16_t v3,int16_t v4);
void set_chassis_6020_angle(float theta0,float theta1,float theta2,float theta3);

typedef struct//    运动参数结构体
{
    //只有底盘，里面可以再加云台
    int16_t Vx;//前后速度
    int16_t Vy;//左右速度
    int16_t Vw;//自转速度 逆时针为正
}Run_Data;

double msp(double x, double in_min, double in_max, double out_min, double out_max);

void all_stop(void);
void stop(void);
void calculateTargetRoundCnt(motor_t* chassis_handle);
void calculateRoundCnt(motor_t* chassis_handle);
void led_red(void);
void led_green(void);
#endif


