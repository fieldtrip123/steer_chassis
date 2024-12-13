#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpragma-pack"
#include "refree.h"
#include "stdio.h"
#include "main.h"
#include "string.h"
#include "usart.h"
#include "crc.h"
game_status_t game_status;
game_result_t game_result;
game_robot_HP_t game_robot_HP;
event_data_t event_data;
ext_supply_projectile_action_t ext_supply_projectile_action;
referee_warning_t referee_warning;
dart_info_t dart_info;
robot_status_t robot_status;
power_heat_data_t power_heat_data;
robot_pos_t robot_pos;
buff_t buff;
air_support_data_t air_support_data;
hurt_data_t hurt_data;
shoot_data_t shoot_data;
projectile_allowance_t projectile_allowance;
rfid_status_t rfid_status;
dart_client_cmd_t dart_client_cmd;
ground_robot_position_t ground_robot_position;
radar_mark_data_t radar_mark_data;
sentry_info_t sentry_info;
radar_info_t radar_info;
robot_interaction_data_t robot_interaction_data;
map_command_t map_command;
map_robot_data_t map_robot_data;
map_data_t map_data;
custom_info_t custom_info;
custom_robot_data_t custom_robot_data;
remote_control_t remote_control;
custom_client_data_t custom_client_data;

uint8_t rawData[137];
//uint32_t last_dwt_tick_;
xFrameHeader FrameHeader;


void decode(uint8_t rawData_t[137])
{
    uint16_t data_length;
    uint16_t cmd_id;

    if(rawData_t==NULL)
    {
        return;
    }


    memcpy(&FrameHeader,rawData_t, LEN_HEADER);

    if (rawData_t[0] == JUDGE_FRAME_HEADER)
    {
        data_length = rawData_t[2] << 8 | rawData_t[1];
        if (Verify_CRC8_Check_Sum(rawData_t, LEN_HEADER))
        {
            if (Verify_CRC16_Check_Sum(rawData_t, LEN_HEADER + LEN_CMDID + data_length + LEN_TAIL))
            {
                cmd_id = (rawData_t[6] << 8 | rawData_t[5]);

                switch (cmd_id) {
                    case ID_game_status:  // 0x0001
                        memcpy(&game_status, (rawData_t + 7), LEN_game_status);
                        break;

                    case ID_game_result:  // 0x0002
                        memcpy(&game_result, (rawData_t + 7), LEN_game_result);
                        break;

                    case ID_game_robot_HP:  // 0x0003
                        memcpy(&game_robot_HP, (rawData_t + 7), LEN_game_robot_HP);
                        break;

                    case ID_event_data:  // 0x0101
                        memcpy(&event_data, (rawData_t + 7), LEN_event_data);
                        break;

                    case ID_supply_projectile_action:  // 0x0102
                        memcpy(&ext_supply_projectile_action, (rawData_t + 7), LEN_supply_projectile_action);
                        break;

                    case ID_referee_warning:  //0x0104
                        memcpy(&referee_warning, (rawData_t + 7), LEN_referee_warning);
                        break;

                    case ID_robot_status:  //0x0201
                        memcpy(&robot_status, (rawData_t + 7), LEN_robot_status);
                        break;

                    case ID_power_heat_data:  //0x0202
                        memcpy(&power_heat_data, (rawData_t + 7), LEN_power_heat_data);
                        break;

                    case ID_robot_pos:  // 0x0203
                        memcpy(&robot_pos, (rawData_t + 7), LEN_robot_pos);
                        break;

                    case ID_buff:  // 0x0204
                        memcpy(&buff, (rawData_t + 7), LEN_buff);
                        break;

                    case ID_air_support_data:  // 0x0205
                        memcpy(&air_support_data, (rawData_t + 7), LEN_air_support_data);
                        break;

                    case ID_hurt_data:  // 0x0206
                        memcpy(&hurt_data, (rawData_t + 7), LEN_hurt_data);
                        break;

                    case ID_shoot_data:  // 0x0207
                        memcpy(&shoot_data, (rawData_t + 7), LEN_shoot_data);
                        break;

                    case ID_projectile_allowance:  // 0x0208
                        memcpy(&projectile_allowance, (rawData_t + 7), LEN_projectile_allowance);
                        break;

                    case LEN_remote_control:  // 0x0304
                        memcpy(&remote_control, (rawData_t + 7), LEN_remote_control);
                        break;

                    default:
                        break;
                }


            }
        }
    }

        if (*(rawData_t + sizeof(xFrameHeader) + LEN_CMDID + data_length + LEN_TAIL) == 0xA5)
        {
            // 如果一个数据包出现了多帧数据,则再次调用解析函数,直到所有数据包解析完毕
            decode(rawData_t + sizeof(xFrameHeader) + LEN_CMDID + data_length + LEN_TAIL);
        }
        //last_dwt_tick_ = DWT->CYCCNT;
}


void RefereeCallback()
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rawData, 256);
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);

    decode(rawData);
}
void referee_init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rawData, 256);
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
}
#pragma clang diagnostic pop