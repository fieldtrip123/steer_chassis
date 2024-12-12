//
// Created by Administrator on 2024/12/3.
//
#include "mec.h"
#include "chassis.h"
#include "math.h"
#include "DR16.h"
#include <stdlib.h>
float wheel_theta[4];
//解算麦轮

extern RC_Ctrl RC_CtrlData;
void move_steer_chassis(int16_t vx,int16_t vy,int16_t vw)
{
    float theta0=atan(1.0/1.0);
    int16_t v1,v2,v3,v4;
    // 12      41
    // 34      32
    v4=(sqrt(pow(vx - vw*RADIUS*sin(theta0),2)
             +  pow(vy + vw*RADIUS*cos(theta0),2)));
    v3=(sqrt(pow(vx - vw*RADIUS*sin(theta0),2)
             +  pow(vy - vw*RADIUS*cos(theta0),2)));
    v1= sqrt(pow(vx + vw*RADIUS*sin(theta0),2)
             +  pow(vy - vw*RADIUS*cos(theta0),2));
    v2= sqrt(pow(vx + vw*RADIUS*sin(theta0),2)
             +  pow(vy + vw*RADIUS*cos(theta0),2));
    set_chassis_3508_speed(v1,v2,-v3,-v4);
}


int16_t send2C;
int16_t con_pitch;


void move_chassis_steer(int16_t vx,int16_t vy,int16_t vw)
{
    //解算目标角度
    //-- +-
    //++ -+
    //4 2
    //3	1
    float theta0;
    theta0=atan(1.0/1.0);   //算一个pi/4出来
    wheel_theta[0] = (atan2((-vy+vw*RADIUS*sin(theta0)),
                            (vx+vw*RADIUS*cos(theta0))))*RAD2DEG;
    wheel_theta[1] = (atan2((-vy-vw*RADIUS*sin(theta0)),
                            (vx+vw*RADIUS*cos(theta0))))*RAD2DEG;
    wheel_theta[2] = (atan2((-vy+vw*RADIUS*sin(theta0)),
                            (vx-vw*RADIUS*cos(theta0))))*RAD2DEG;
    wheel_theta[3] = (atan2((-vy-vw*RADIUS*sin(theta0)),
                            (vx-vw*RADIUS*cos(theta0))))*RAD2DEG;


    //这里都是角度制
    set_chassis_6020_angle(wheel_theta[0],wheel_theta[1],wheel_theta[2],wheel_theta[3]);
    send2C= abs(wheel_theta[0]);
}

