//
// Created by Administrator on 2024/12/13.
//

#ifndef SUPERCAP_H
#define SUPERCAP_H

#include "main.h"
typedef struct
{
uint16_t  current_energy;
uint16_t  current_input_power;
uint16_t  current_chassis_power;

} supercap_recv_t;

void send2supercap( int16_t tx2cap1,int16_t tx2cap2,int16_t tx2cap3,int16_t tx2cap4);


#endif
