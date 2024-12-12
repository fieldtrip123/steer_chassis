//
// Created by Administrator on 2024/12/4.
//

#include "refree.h"
#include "main.h"
#include "usart.h"
#include "string.h"
RC_ID robot_client_ID;
static const unsigned char CRC8_TAB[256] = {
        0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3,
        0xfd, 0x1f, 0x41, 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01,
        0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe,
        0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 0xbe, 0xe0, 0x02, 0x5c,
        0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 0x46,
        0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb,
        0x59, 0x07, 0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5,
        0xfb, 0x78, 0x26, 0xc4, 0x9a, 0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
        0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 0xf8, 0xa6, 0x44, 0x1a, 0x99,
        0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 0x8c, 0xd2,
        0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93,
        0xcd, 0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31,
        0xb2, 0xec, 0x0e, 0x50, 0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d,
        0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d,
        0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 0xca, 0x94, 0x76,
        0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
        0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4,
        0xaa, 0x48, 0x16, 0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75,
        0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9,
        0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};

/* 16位CRC校验码，用于数据帧整帧校验 */
static const uint16_t wCRC_Table[256] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e,
        0xf8f7, 0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64,
        0xf9ff, 0xe876, 0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e,
        0xfae7, 0xc87c, 0xd9f5, 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50,
        0xfbef, 0xea66, 0xd8fd, 0xc974, 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 0xce4c, 0xdfc5, 0xed5e,
        0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44,
        0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e,
        0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e,
        0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324,
        0xf1bf, 0xe036, 0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e,
        0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 0xb58b, 0xa402, 0x9699, 0x8710,
        0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 0xc60c, 0xd785, 0xe51e,
        0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 0xd68d, 0xc704,
        0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 0xe70e,
        0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1,
        0x0f78
};
/* 比赛接收数据 */
 ext_game_status_t GameState;                                                    //比赛状态数据
 ext_game_result_t GameResult;                                                    //比赛结果数据
 ext_game_robot_HP_t GameRobotHP;                                                //机器人血量数据
 ext_event_data_t EventData;                                                        //场地事件数据
 ext_supply_projectile_action_t SupplyAction;                                    //补给站动作数据
 ext_referee_warning_t RefereeWarning;                                            //裁判警告信息数据
 ext_dart_remaining_time_t DartRemainTime;                                        //飞镖发射倒计时数据
 ext_game_robot_status_t GameRobotState;                                            //机器人状态：当前血量、射速、底盘功率等
 ext_power_heat_data_t PowerHeatData;                                            //机器人功率、热量数据
 ext_game_robot_pos_t RobotPos;                                                    //机器人位置数据
 ext_buff_t RobotBuff;                                                            //机器人增益数据
 aerial_robot_energy_t AerialEnergy;                                                //空中机器人能量状态数据
 ext_robot_hurt_t RobotHurt;                                                        //机器人伤害状态数据
 ext_shoot_data_t ShootData;                                                        //实时射击信息数据
 ext_bullet_remaining_t BulletRemaining;                                            //剩余弹丸量数据
 ext_rfid_status_t RFID_Status;                                                    //RFID状态数据
 ext_dart_client_cmd_t DartClientCmd;                                            //飞镖机器人客户端指令数据

/* 交互数据 */
my_interactive_data_t robot_rec_data[9];										//存储本方其他机器人发送的信息，添加了雷达站
custom_controller_interactive_data_t custom_control_data;						//自定义控制器数据段部分
ext_mini_map_command_t mini_map_data;											//小地图下发信息
ext_VT_command_t VT_command_data;												//键鼠信息，通过图传发送端接收
ext_client_map_command_t radar_map_data;										//雷达站信息

void REFREE_Init(void)//初始化
{
HAL_UARTEx_ReceiveToIdle_DMA(&huart2,UART_REFREE,256);
__HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
}
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC)
{
uint8_t chData;
if (pchMessage == NULL) {
 return 0xFFFF;
}
while(dwLength--) {
 chData = *pchMessage++;
 (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table[((uint16_t)(wCRC) ^ (uint16_t)(chData)) & 0x00ff];
}
return wCRC;
}
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8)
{
unsigned char ucIndex;
while (dwLength--) {
 ucIndex = ucCRC8^(*pchMessage++);
 ucCRC8 = CRC8_TAB[ucIndex];
}
return(ucCRC8);
}

void unPackDataFromRF(uint8_t *data_buf, uint32_t length)
{
static uint8_t RFdataBuff[256];														//将data_buf指向的数据帧复制到缓存区中
static int32_t index,buff_read_index;												//解包结果存储区索引，及缓存区索引
static short CRC16_Function,CRC16_Referee;											//记录整帧的CRC16计算结果，及帧末的CRC16数值
static uint8_t byte;                                                                //记录数据长度
static int32_t read_len;															//记录数据帧总体字节数
static uint16_t data_len;															//记录数据帧的数据段字节数
static uint8_t unpack_step;															//记录当前数据帧的解包状态
static uint8_t protocol_packet[PROTOCAL_FRAME_MAX_SIZE];							//解包结果存储区

/*初始化读取状态*/
    buff_read_index = 0;

 //让RFdataBuff 接受  UART_REFREE的数据
    memcpy(RFdataBuff,data_buf,length);
    /*从数据帧帧头开始读取 */
    read_len=length;

    while (read_len--)
    {
        byte = RFdataBuff[buff_read_index++];											//载入缓存区当前数据
        switch(unpack_step) {
            case STEP_HEADER_SOF: {
                if(byte == START_ID) {
                    unpack_step = STEP_LENGTH_LOW;											//将数据帧帧头的SOF写入结果存储区
                    protocol_packet[index++] = byte;
                } else {
                    index = 0;
                }
            }
                break;

            case STEP_LENGTH_LOW: {
                data_len = byte;
                protocol_packet[index++] = byte;											//将数据帧帧头的len低八位写入结果存储区
                unpack_step = STEP_LENGTH_HIGH;
            }
                break;

            case STEP_LENGTH_HIGH: {
                data_len |= (byte << 8);
                protocol_packet[index++] = byte;
                if(data_len < (PROTOCAL_FRAME_MAX_SIZE - HEADER_LEN - CRC_ALL_LEN)) {
                    unpack_step = STEP_FRAME_SEQ;											//将数据帧帧头的len高八位写入结果存储区
                } else {
                    unpack_step = STEP_HEADER_SOF;											//若数据帧过长，则放弃接收该帧
                    index = 0;
                }
            }
                break;

            case STEP_FRAME_SEQ: {
                protocol_packet[index++] = byte;											//将数据帧帧头的包序列SEQ写入结果存储区
                unpack_step = STEP_HEADER_CRC8;
            }
                break;

            case STEP_HEADER_CRC8: {														//帧头CRC8查表校验
                protocol_packet[index++] = byte;											//将CRC8校验码写入结果缓存区

                if (index == HEADER_LEN+1) {
                    if ( Get_CRC8_Check_Sum(protocol_packet, HEADER_LEN,0xff)== protocol_packet[HEADER_LEN]) {
                        unpack_step = STEP_DATA_CRC16;										//帧头校验成功，则进入整帧校验
                    } else {
                        unpack_step = STEP_HEADER_SOF;
                        index = 0;
                    }
                }
            }
                break;

            case STEP_DATA_CRC16: {
                if (index < (HEADER_LEN + CMD_LEN + data_len + CRC_ALL_LEN)) {				//未访问完整帧字节
                    protocol_packet[index++] = byte;
                }
                if (index >= (HEADER_LEN + CMD_LEN + data_len + CRC_ALL_LEN)) {				//访问完整帧，接下来开始CRC16校验

                    CRC16_Function=Get_CRC16_Check_Sum(protocol_packet, HEADER_LEN + CMD_LEN + data_len +CRC_8_LEN,0xffff);
                    CRC16_Referee=* ( short *)(&protocol_packet[index-2]);			//取出数据帧帧末的CRC16校验和
                    if ( CRC16_Function==CRC16_Referee) {									//对比CRC校验结果
                        RefereeHandle(protocol_packet);										//校验完成，将解包结果按照cmd_id转存到类的对应成员结构体中
                    }
                    unpack_step = STEP_HEADER_SOF;
                    index = 0;
                }
            }
                break;

            default: {																		//其他情况则直接重置解包状态和索引
                unpack_step = STEP_HEADER_SOF;
                index = 0;
            }
                break;
        }
    }
}
/**
  * @brief  Calculate robot ID
  * @param  local_id: 裁判系统发送的本机器人ID
  * @retval None
  */
void Calc_Robot_ID(uint8_t local_id)
{
    uint8_t *id_ptr = (uint8_t*)&robot_client_ID;
    uint8_t i = 1;

    if(local_id !=0 )
    {
        if(local_id < 10)																//计算当前机器人的ID号（红方）
        {
            for(i = 1;i < 10;i++)
                (*id_ptr++) = i;

            robot_client_ID.robot_where = Robot_Red;
            robot_client_ID.local = local_id;
            robot_client_ID.client = 0x100 + local_id;
        }
        else																			//计算当前机器人的ID号（蓝方）
        {
            for(i = 1;i < 10;i++)
                (*id_ptr++) = i + 100;

            robot_client_ID.robot_where = Robot_Blue;
            robot_client_ID.local = local_id;
            robot_client_ID.client = 0x0100 + local_id;
        }
    }
}

/**
  * @brief  Receive and handle referee system data
  * @param  *data_buf:data which is unpacked successfully
  * @retval void
  */
void RefereeHandle(uint8_t *data_buf)
{
    switch(((FrameHeader *)data_buf)->CmdID) {											//取出解包结果的cmd_id

        case GameState_ID:
            GameState = *(ext_game_status_t*)(&data_buf[7]);						//取解包结果数据段的段起始字节地址，转化为对应结构体类型的指针，并对该指针取值，即得数据
            break;

        case GameResult_ID:
            GameResult = *(ext_game_result_t*)(&data_buf[7]);
            break;

        case GameRobotHP_ID:
            GameRobotHP = *(ext_game_robot_HP_t*)(&data_buf[7]);
            break;

        case EventData_ID:
            EventData = *(ext_event_data_t*)(&data_buf[7]);
            break;

        case SupplyProjectileAction_ID:
            SupplyAction = *(ext_supply_projectile_action_t*)(&data_buf[7]);
            break;

        case RefereeWarning_ID:
            RefereeWarning = *(ext_referee_warning_t*)(&data_buf[7]);
            break;

        case DartRemainingTime_ID:
            DartRemainTime = *(ext_dart_remaining_time_t*)(&data_buf[7]);
            break;

        case GameRobotState_ID:
            GameRobotState = *(ext_game_robot_status_t*)(&data_buf[7]);
            Calc_Robot_ID(GameRobotState.robot_id);
            break;

        case PowerHeatData_ID:
            PowerHeatData = *(ext_power_heat_data_t*)(&data_buf[7]);
            break;

        case GameRobotPos_ID:
            RobotPos = *(ext_game_robot_pos_t*)(&data_buf[7]);
            break;

        case BuffMusk_ID:
            RobotBuff = *(ext_buff_t*)(&data_buf[7]);
            break;

        case AerialRobotEnergy_ID:
            AerialEnergy = *(aerial_robot_energy_t*)(&data_buf[7]);
            break;

        case RobotHurt_ID:
            RobotHurt = *(ext_robot_hurt_t*)(&data_buf[7]);
            break;

        case ShootData_ID:
            ShootData = *(ext_shoot_data_t*)(&data_buf[7]);
            break;

        case BulletRemaining_ID:
            BulletRemaining = *(ext_bullet_remaining_t*)(&data_buf[7]);
            break;

        case RFID_Status_ID:
            RFID_Status = *(ext_rfid_status_t*)(&data_buf[7]);
            break;

        case ExtDartClientCmd_ID:
            DartClientCmd = *(ext_dart_client_cmd_t*)(&data_buf[7]);
            break;

        case StudentInteractiveHeaderData_ID:																//若为数据交互的数据帧，则交给其他成员函数进行处理
            RobotInteractiveHandle((robot_interactive_data_t*)(&data_buf[7]));							//robot_interactive_data_t是整个数据段！
            break;

        case CustomControllerData_ID:
            custom_control_data = *(custom_controller_interactive_data_t*)(&data_buf[7]);
            break;

        case MiniMapInteractiveData_ID:
            mini_map_data = *(ext_mini_map_command_t*)(&data_buf[7]);
            break;
        default:
            break;
    }
}
/**
  * @brief  处理车间通信数据包
  * @param  RobotInteractiveData_t:车间通信数据包中数据段首地址
  * @retval None
  */
void RobotInteractiveHandle(robot_interactive_data_t* RobotInteractiveData_t)
{
    if(GameRobotState.robot_id == RobotInteractiveData_t->receiver_ID && GameRobotState.robot_id != 0) {		//如果确实是发往本机器人的交互数据帧，则接受该帧
        if(RobotInteractiveData_t->data_cmd_id == RobotComData_ID) {											//根据发送者的ID号归纳数据段并存储到对应区域
            if(RobotInteractiveData_t->sender_ID > 100)															//蓝方数据接收
                memcpy(&robot_rec_data[RobotInteractiveData_t->sender_ID - 101], RobotInteractiveData_t->data, ROBOT_COM_PACK);
            else																								//红方数据接收
                memcpy(&robot_rec_data[RobotInteractiveData_t->sender_ID - 1], RobotInteractiveData_t->data, ROBOT_COM_PACK);
        }
    }
}