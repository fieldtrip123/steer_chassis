//
// Created by Administrator on 2024/12/3.
//

#include "pid.h"

pid_struct_t chassis_3508_pid[4];
pid_struct_t chassis_speed_pid[4];
pid_struct_t chassis_angle_pid[4];



void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)
{
    pid->kp      = kp;
    pid->ki      = ki;
    pid->kd      = kd;
    pid->i_max   = i_max;
    pid->out_max = out_max;
}

/**
  * @brief  pid calculation
  * @param  pid struct
    @param  reference value
    @param  feedback value
  * @retval calculation result
  */
float pid_calc(pid_struct_t *pid, float ref, float fdb)
{
    pid->ref = ref;
    pid->fdb = fdb;
    pid->err[1] = pid->err[0];
    pid->err[0] = pid->ref - pid->fdb;

    pid->p_out  = pid->kp * pid->err[0];
    pid->i_out += pid->ki * pid->err[0];
    pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
    LIMIT_MIN_MAX(pid->i_out, -pid->i_max, pid->i_max);

    pid->output = pid->p_out + pid->i_out + pid->d_out;
    LIMIT_MIN_MAX(pid->output, -pid->out_max, pid->out_max);
    return pid->output;
}


void all_pid_init()
{
    for (int i =0;i<4;i++)
    {
        pid_init(&chassis_speed_pid[i], 90, 0, 0,16000, 16000);//P=85,I=0,D=0

    }
    pid_init(&chassis_angle_pid[0], 7, 0, 8, -100, 100);//P=1,I=0,D=0
    pid_init(&chassis_angle_pid[1], 2, 0, 0, -100, 100);//P=1,I=0,D=0
    pid_init(&chassis_angle_pid[2], 2, 0, 5, -100, 100);//P=1,I=0,D=0
    pid_init(&chassis_angle_pid[3], 2, 0, 0, -100, 100);//P=1,I=0,D=0
    for (int i =0;i<4;i++)
    {
        pid_init(&chassis_3508_pid[i], 25, 0, 0, 30000, 30000);
    }
}


