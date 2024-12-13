//
// Created by Administrator on 2024/12/13.
//

#include "supercap.h"
#include "mycan.h"
#include "can.h"

void send2supercap( int16_t tx2cap1,int16_t tx2cap2,int16_t tx2cap3,int16_t tx2cap4)
{
    CAN_TxHeaderTypeDef  tx_header4;
    uint8_t              tx_data4[8];

    tx_header4.StdId= 0x102;
    tx_header4.DLC =8;
    tx_header4.IDE=CAN_ID_STD;
    tx_header4.RTR=CAN_RTR_DATA;

    tx_data4[0]=   (tx2cap1)>>8 &  0xff;
    tx_data4[1]=    tx2cap1     &  0xff;
    tx_data4[2]=   (tx2cap2)>>8 &  0xff;
    tx_data4[3]=    tx2cap2    &  0xff;
    tx_data4[4]=   (tx2cap3)>>8  &  0xff;
    tx_data4[5]=    tx2cap3      &  0xff;
    tx_data4[6]=   (tx2cap4)>>8  &  0xff;
    tx_data4[7]=    tx2cap4      &  0xff;

    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0)
    {
        HAL_CAN_AddTxMessage(&hcan1, &tx_header4, tx_data4, (uint32_t *)CAN_TX_MAILBOX1);
    }
}

