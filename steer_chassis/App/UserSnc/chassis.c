//
// Created by Administrator on 2024/12/3.
//

#include "chassis.h"
#include "pid.h"
#include "mycan.h"
#include "math.h"
#include "string.h"
#include "DR16.h"

#define    encode0  6063 // 6247
#define    encode1  8890//4827
#define	   encode2  8778//6180
#define    encode3  4775//6196
extern   motor_t  motor_chassis_3508_info[4];
extern   motor_t  motor_chassis_6020_info[4];
extern   pid_struct_t chassis_3508_pid[4];
extern   pid_struct_t chassis_speed_pid[4];
extern   pid_struct_t chassis_angle_pid[4];

float    target_chassis_speed[4];
float    target_chassis_enc[4];
float    now_enc[4];


/*double msp(double x, double in_min, double in_max, double out_min, double out_max)//映射函数，将编码器的值（0~8191）转换为弧度制的角度值（-pi~pi）
{
    return (x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
}*/


void set_chassis_3508_speed(int16_t v1,int16_t v2,int16_t v3,int16_t v4)
{
    //每个电机的目标值
    target_chassis_speed[0]=v1*M3508_REDUCTION_RATIO;
    target_chassis_speed[1]=v2*M3508_REDUCTION_RATIO;
    target_chassis_speed[2]=v3*M3508_REDUCTION_RATIO;
    target_chassis_speed[3]=v4*M3508_REDUCTION_RATIO;

    for(int i=0;i<4;i++)
    {
        motor_chassis_3508_info[i].set_current=pid_calc(&chassis_3508_pid[i], target_chassis_speed[i], motor_chassis_3508_info[i].rotor_speed);
    }
    //  4个电机赋速度值
    set_chassis_3508_v(
            motor_chassis_3508_info[0].set_current,
            motor_chassis_3508_info[1].set_current,
            motor_chassis_3508_info[2].set_current,
            motor_chassis_3508_info[3].set_current
    );
}

void set_chassis_6020_angle(float theta0,float theta1,float theta2,float theta3)
{

    //每个电机的目标值
    //41
    //32
    if     ((motor_chassis_6020_info[0].last_target-theta0*DEG2ENC)>4096)
    {
        motor_chassis_6020_info[0].target_count++;
    }
    else if ((motor_chassis_6020_info[0].last_target-theta0*DEG2ENC)<-4096)
    {
        motor_chassis_6020_info[0].target_count--;
    }
    motor_chassis_6020_info[0].last_target = theta0;

    if       ((motor_chassis_6020_info[0].last_target-theta0*DEG2ENC)>4096)
    {
        motor_chassis_6020_info[0].target_count++;
    }
    else if ((motor_chassis_6020_info[1].last_target-theta1*DEG2ENC)<-4096)
    {
        motor_chassis_6020_info[1].target_count--;
    }
    motor_chassis_6020_info[1].last_target = theta1;

    if       ((motor_chassis_6020_info[2].last_target-theta2*DEG2ENC)>4096)
    {
        motor_chassis_6020_info[2].target_count++;
    }
    else if ((motor_chassis_6020_info[2].last_target-theta2*DEG2ENC)<-4096)
    {
        motor_chassis_6020_info[2].target_count--;
    }
    motor_chassis_6020_info[2].last_target = theta2;

    if       ((motor_chassis_6020_info[3].last_target-theta3*DEG2ENC)>4096)
    {
        motor_chassis_6020_info[3].target_count++;
    }
    else if ((motor_chassis_6020_info[3].last_target-theta3*DEG2ENC)<-4096)
    {
        motor_chassis_6020_info[3].target_count--;
    }
    motor_chassis_6020_info[3].last_target = theta3;

    motor_chassis_6020_info[0].add_target=(motor_chassis_6020_info[0].target_count)*8192+theta0*DEG2ENC+encode0;
    motor_chassis_6020_info[1].add_target=(motor_chassis_6020_info[1].target_count)*8192+theta1*DEG2ENC+encode1;
    motor_chassis_6020_info[2].add_target=(motor_chassis_6020_info[2].target_count)*8192+theta2*DEG2ENC+encode2;
    motor_chassis_6020_info[3].add_target=(motor_chassis_6020_info[3].target_count)*8192+theta3*DEG2ENC+encode3;

    target_chassis_enc[0]=(int)motor_chassis_6020_info[0].add_target;
    target_chassis_enc[1]=(int)motor_chassis_6020_info[1].add_target;
    target_chassis_enc[2]=(int)motor_chassis_6020_info[2].add_target;
    target_chassis_enc[3]=(int)motor_chassis_6020_info[3].add_target;

    now_enc[0]=motor_chassis_6020_info[0].add_encode  ;
    now_enc[1]=motor_chassis_6020_info[1].add_encode  ;
    now_enc[2]=motor_chassis_6020_info[2].add_encode  ;
    now_enc[3]=motor_chassis_6020_info[3].add_encode  ;

    //pid  控制
    float angle;
    for(int i=0;i<4;i++)
    {
        angle =  pid_calc(&chassis_angle_pid[i],target_chassis_enc[i], now_enc[i]);//角度环
        pid_calc(&chassis_speed_pid[i],angle, motor_chassis_6020_info[i].rotor_speed);
    }
    //  4个电机赋电压值
    set_chassis_6020_v
            (
                    chassis_speed_pid[0].output,
                    chassis_speed_pid[1].output,
                    chassis_speed_pid[2].output,
                    chassis_speed_pid[3].output
            );
}


void stop(void)
{
    set_chassis_3508_v(0,0,0,0);
}

void all_stop(void)
{
    set_chassis_6020_v(0,0,0,0);
    set_chassis_3508_v(0,0,0,0);
}
void led_red(void)
{
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
}
void led_green(void)
{
    HAL_GPIO_TogglePin(LED_GREEEN_GPIO_Port,LED_GREEEN_Pin);
}

