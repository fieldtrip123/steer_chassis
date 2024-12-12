//
// Created by Administrator on 2024/12/3.
//

#ifndef  __MYCAN__H
#define  __MYCAN__H

#define  num_chassis_m3508 4
#define  num_chassis_m6020 4
#define  FEEDBACK_ID_CHASSIS_3508_BASE 0x201  //0x1ff
#define  FEEDBACK_ID_CHASSIS_6020_BASE 0x205  //0x1ff
#include "can.h"
#include "main.h"

typedef struct {

    uint16_t can_id;         //id
    int16_t  set_current;    //设定电流
    uint16_t rotor_angle;    //机械角度
    int16_t  rotor_speed;    //转速
    int16_t  torque_current; //扭矩电流
    uint8_t  temp;           //温度
    int    circle_count;
    int	   target_count;
    int16_t  add_target;
    int16_t  add_encode;
    int16_t  last_encode;
    int16_t  last_target;
}    motor_t;


void my_can1_init(void);
void my_can2_init(void);
void set_chassis_3508_v(int16_t v1, int16_t v2, int16_t v3, int16_t v4);
void set_chassis_6020_v(int16_t v5, int16_t v6, int16_t v7, int16_t v8);
void send_msg( int16_t tx2c1,int16_t tx2c2,int16_t flag1,int16_t flag2);


#endif

