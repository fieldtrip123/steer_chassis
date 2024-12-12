//
// Created by Administrator on 2024/12/4.
//

#ifndef REFREE_H
#define REFREE_H

#include "main.h"
extern uint8_t UART_REFREE[256];
void REFREE_Init(void);

#define DRAWING_PACK	15
#define	ROBOT_COM_PACK	8
/* Private variables ---------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
/* 裁判系统状态；0：掉线，1：在线 */
enum RF_status_e
{
	RF_OFFLINE = 0U,
	RF_ONLINE
};

/* 枚举变量，用于车间通信数组访问 */
enum{
	HERO = 0U,
	ENGINEER,
	INFANTRY_3,
	INFANTRY_4,
	INFANTRY_5,
	AERIAL,
	SENTRY,
	RADAR = 8U
};

enum{
	Robot_Red = 0U,
	Robot_Blue
};

/* 己方机器人ID及当前机器人客户端ID */
typedef	struct {
	uint8_t hero;
	uint8_t engineer;
	uint8_t infantry_3;
	uint8_t infantry_4;
	uint8_t infantry_5;
	uint8_t aerial;
	uint8_t sentry;
	uint8_t drat;														//飞镖发射架
	uint8_t radar;														//雷达站
	uint8_t local;														//本机器人ID

	uint8_t robot_where;												//蓝方 or 红方机器人
	uint16_t client;													//对应的客户端ID
} __attribute__((packed))RC_ID;

/* 数据帧帧头结构体 */
typedef struct {
	uint8_t SOF;
	uint16_t DataLength;
	uint8_t Seq;
	uint8_t CRC8;
	uint16_t CmdID;
} __attribute__((packed))FrameHeader;

/* 交互数据帧的统一数据段段头 */
#pragma pack(push, 1)
typedef struct {
	uint16_t data_cmd_id;
	uint16_t send_ID;
	uint16_t receiver_ID;
} __attribute__((packed)) DataHeader;
#pragma pack(pop)
/* ----------------------------------各种数据类型对应的ID号--------------------------------------- */
typedef enum {
	/* 裁判系统发过来数据帧ID类型，注意这里是CMD_ID */
	GameState_ID                    = 0x0001,		//比赛状态数据 1Hz
	GameResult_ID                   = 0x0002,		//比赛结果数据 结束后发送
	GameRobotHP_ID                  = 0x0003,		//机器人血量数据 1Hz
	EventData_ID                    = 0x0101,		//场地事件数据 1Hz
	SupplyProjectileAction_ID       = 0x0102,		//补给站动作标识 动作后发送
	RefereeWarning_ID               = 0x0104,		//裁判警告数据 警告后发送
	DartRemainingTime_ID            = 0x0105,		//飞镖发射口倒计时 1Hz
	GameRobotState_ID               = 0x0201,		//机器人状态 10Hz
	PowerHeatData_ID                = 0x0202,		//功率热量数据 50Hz
	GameRobotPos_ID                 = 0x0203,		//机器人位置数据 10Hz
	BuffMusk_ID                     = 0x0204,		//机器人增益 状态改变后发送
	AerialRobotEnergy_ID            = 0x0205,		//空中机器人能量 10Hz
	RobotHurt_ID                    = 0x0206,		//伤害数据 伤害发生后发送
	ShootData_ID                    = 0x0207,   	//实时射击数据 发射后发送
	BulletRemaining_ID              = 0x0208,		//子弹剩余发送数 哨兵及空中 1Hz
	RFID_Status_ID           		= 0x0209,		//RFID状态 1Hz
	ExtDartClientCmd_ID             = 0x020D,		//哨兵机器人客户端发送指令 10Hz
	StudentInteractiveHeaderData_ID	= 0x0301, 		//机器人交互数据 30Hz上限
	CustomControllerData_ID			= 0x0302,	 	//自定义控制器交互数据接口 30Hz
	MiniMapInteractiveData_ID		= 0x0303,		//小地图交互数据 交互发送
	VT_Data_ID						= 0x0304,		//键盘、鼠标数据，通过图传发送
	ClientMapCommand_ID				= 0x0305,		//小地图接收信息

	/* 机器人交互数据段的ID类型，注意这里是数据段内容ID！ */
	RobotComData_ID					= 0x0233,		//车间交互，该ID由各个队伍自定义
	CustomData_ID					= 0xD180,		//自定义数据ID
	Drawing_Clean_ID				= 0x0100,
	Drawing_1_ID					= 0x0101,
	Drawing_2_ID					= 0x0102,
	Drawing_5_ID					= 0x0103,
	Drawing_7_ID					= 0x0104,
	Drawing_Char_ID					= 0x0110,

} RefereeSystemID_t;

/* ----------------------------------比赛数据帧的数据段框架--------------------------------------- */
/**
  @brief  比赛状态数据：0x0001，1Hz
*/
typedef struct {
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
} __attribute__((packed))ext_game_status_t;

/**
   @brief 比赛结果数据：0x0002，在比赛结束后发送
*/
typedef struct {
  uint8_t winner;
} ext_game_result_t;

/**
   @brief 机器人血量数据：0x0003，1Hz
*/
typedef struct {
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;

	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
} ext_game_robot_HP_t;


/**
   @brief 场地事件数据：0x0101，事件改变发送
*/
typedef struct {
  uint32_t event_type;
} ext_event_data_t;

/**
   @brief 补给站动作标识：0x0102	动作改变后发送
*/
typedef struct {
  uint8_t supply_projectile_id;
  uint8_t supply_robot_id;
  uint8_t supply_projectile_step;
  uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

/**
   @brief 裁判警告信息：0x0104	警告发生后发送
*/
typedef struct {
	uint8_t level;
	uint8_t foul_robot_id;
} ext_referee_warning_t;

/**
   @brief 飞镖发射口倒计时：0x0105，1Hz
*/
typedef struct
{
 uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

/**
   @brief 当前比赛机器人状态：0x0201，10Hz
*/
typedef struct {
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t remain_HP;
	uint16_t max_HP;
	uint16_t shooter_id1_17mm_cooling_rate;/* 17mm */
	uint16_t shooter_id1_17mm_cooling_limit;
	uint16_t shooter_id1_17mm_speed_limit;

	uint16_t shooter_id2_17mm_cooling_rate;
	uint16_t shooter_id2_17mm_cooling_limit;
	uint16_t shooter_id2_17mm_speed_limit;

	uint16_t shooter_id1_42mm_cooling_rate;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint16_t shooter_id1_42mm_speed_limit;

	uint16_t classis_power_limit;
	uint8_t mains_power_gimbal_output : 1;
	uint8_t mains_power_chassis_output : 1;
	uint8_t mains_power_shooter_output : 1;
} __attribute__((packed))ext_game_robot_status_t;

/**
   @brief 实时功率热量数据：0x0202，50Hz
*/
typedef struct {
	uint16_t chassis_volt;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t chassis_power_buffer;
	uint16_t shooter_id1_17mm_cooling_heat;
	uint16_t shooter_id2_17mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_heat;
} __attribute__((packed))ext_power_heat_data_t;

/**
   @brief 机器人位置：0x0203,10Hz
*/
#pragma pack(push, 1)
typedef struct {
  float x;
  float y;
  float z;
  float yaw;
}  __attribute__((packed))ext_game_robot_pos_t;
#pragma pack(pop)
/**
   @brief 机器人增益：0x0204，状态改变后发送
*/
typedef struct {
  uint8_t power_rune_buff;
} ext_buff_t;

/**
   @brief 空中机器人能量状态：0x0205，10Hz
*/
typedef struct {
  uint8_t attack_time;
} aerial_robot_energy_t;

/**
   @brief 伤害状态：0x0206，受到伤害后发送
*/
typedef struct {
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;

/**
   @brief 实时射击信息：0x0207，射击后发送
*/
typedef struct {
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
} __attribute__((packed))ext_shoot_data_t;

/**
   @brief 子弹剩余发射数：0x0208，10Hz
*/
typedef struct {
  	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

/**
   @brief 机器人RFID状态：0x0209，1Hz
*/
typedef struct
{
  uint32_t rfid_status;  /* 每个位代表不同地点的RFID状态 */
} ext_rfid_status_t;	/*RFID状态不完全代表对应的增益或处罚状态，例如敌方已占领的高低增益点，不能获取对应的增益效果*/

/**
	@brief 飞镖机器人客户端指令数据：0x020A，10Hz 只对飞镖发送
*/
typedef struct {
  	uint8_t dart_launch_opening_status;
	uint8_t dart_attack_target;
	uint16_t target_change_time;
	uint16_t operate_launch_cmd_time;
} __attribute__((packed))ext_dart_client_cmd_t;


/* ----------------------------------裁判系统客户端交互部分--------------------------------------- */
/**
   @brief 交互数据段通用段头结构体定义：自定义数据的命令ID为：0x0301，10Hz
*/
typedef struct {
  uint16_t data_cmd_id;
  uint16_t sender_ID;
  uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

/**
   @brief 学生机器人间通信：命令ID为0x0301，内容ID：在0x0200~0x02FF内自由选择，10Hz
   @brief 这里定义了完整的数据段格式，虽然看起来有些冗余，但方便处理其他机器人的发过来的数据报
*/
typedef struct {
	uint16_t data_cmd_id;												//数据段段首
	uint16_t sender_ID;
	uint16_t receiver_ID;
	uint8_t data[112];           										//!<长度需要小于113个字节（官方手册约定）
} __attribute__((packed))robot_interactive_data_t;

typedef struct {
	uint8_t data[ROBOT_COM_PACK];           							//接收到的数据
} my_interactive_data_t;

/**
   @brief 自定义控制器交互数据：0x0302，30Hz；
   @brief 注意！该交互数据包数据段没有段首，直接就是数据部分
*/
typedef struct {
	uint8_t data[30];           										//!<长度需要小于30个字节（官方手册约定）
} custom_controller_interactive_data_t;

/**
   @brief 小地图下发数据：0x0303 触发时发送；
*/
typedef struct
{
	float target_position_x;
	float target_position_y;
	float target_position_z;
	uint8_t commd_keyboard;
	uint16_t target_robot_ID;
} __attribute__((packed))ext_mini_map_command_t;

/**
   @brief 键鼠信息，通过图传发送到机器人（图传发送端的3pin接口）：0x0304；
*/
typedef struct
{
	int16_t mouse_x;
	int16_t mouse_y;
	int16_t mouse_z;
	int8_t left_button_down;
	int8_t right_button_down;
	uint16_t keyboard_value;
	uint16_t reserved;
} __attribute__((packed))ext_VT_command_t;

/**
   @brief 雷达站下发数据帧内容定义，将被己方操作手以第一人称看到，0x305
*/
typedef struct
{
	uint16_t target_robot_ID;											//敌方机器人的坐标
	float target_position_x;
	float target_position_y;
	float toward_angle;
} __attribute__((packed))ext_client_map_command_t;
/* ID: 0x0305  Byte:  10      选手端小地图接收雷达数据*/
typedef struct
{
    uint16_t target_robot_id;
    float target_position_x;
    float target_position_y;
} map_robot_data_t;

/* ID: 0x0307  Byte:  103     选手端小地图接收哨兵数据*/
typedef struct
{
    uint8_t intention;
    uint16_t start_position_x;
    uint16_t start_position_y;
    int8_t delta_x[49];
    int8_t delta_y[49];
    uint16_t sender_id;
} map_data_t;

/* ID: 0x0308  Byte:  34     选手端小地图接收机器人数据*/
typedef struct
{
    uint16_t sender_id;
    uint16_t receiver_id;
    uint8_t user_data[30];
} custom_info_t;

/* ID: 0x0302  Byte:  30     自定义控制器与机器人交互数据*/
typedef struct
{
    uint8_t data[30];
} custom_robot_data_t;

/* ID: 0x0304  Byte:  12     键鼠遥控数据*/
typedef struct
{
    int16_t mouse_x;
    int16_t mouse_y;
    int16_t mouse_z;
    int8_t left_button_down;
    int8_t right_button_down;
    uint16_t keyboard_value;
    uint16_t reserved;
} remote_control_t;

/* ID: 0x0306  Byte:  8     自定义控制器与选手端交互数据*/
typedef struct
{
    uint16_t key_value;
    uint16_t x_position : 12;
    uint16_t mouse_left : 4;
    uint16_t y_position : 12;
    uint16_t mouse_right : 4;
    uint16_t reserved;
} custom_client_data_t;

/* ----------------------------------解包用到的宏定义、枚举量--------------------------------------- */
#define START_ID	0XA5												//数据帧的起始ID，官方约定
#define PROTOCAL_FRAME_MAX_SIZE	60 										//这个参数在解包时只作为大致的数据包长度判断，可以设置为 >= 最大长度
#define HEADER_LEN 	4													//数据帧帧头长度（为5），4是为了方便直接用数组访问
#define CRC_ALL_LEN	3													//CRC校验码长度
#define CRC_8_LEN	1
#define CMD_LEN	2														//数据帧指令ID

/* 数据帧解包状态 */
typedef enum {
    STEP_HEADER_SOF=0,													//帧头SOF，应为0xA5
    STEP_LENGTH_LOW,													//数据段长度低8位
    STEP_LENGTH_HIGH,													//数据段长度高8位
    STEP_FRAME_SEQ,														//包序号
    STEP_HEADER_CRC8,													//帧头CRC8校验码
    STEP_DATA_CRC16														//帧末CRC16校验码
} unPackStep_e;

/* 交互数据帧的通信类型，机器人间通信 or 自定义UI or 小地图通信 */
typedef enum
{
	CV_OtherRobot = 0U,
	UI_Client,
	MiniMap_Client
}receive_Type_e;

typedef uint32_t (*SystemTick_Fun)(void);

/* ----------------------------------裁判系统串口通信类--------------------------------------- */
        uint8_t DataCheck(uint8_t **p);
        void unPackDataFromRF(uint8_t *data_buf, uint32_t length);                        //数据帧整帧解包
        void RefereeHandle(uint8_t *data_buf);
        unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);		//获取数据包包头的CRC8校验和
        uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);							//获取数据包整包的CRC16校验和//更新对应ID的数据
        void RobotInteractiveHandle(robot_interactive_data_t* RobotInteractiveData_t);								//机器人间通信
        /* 发送时用到的 */
        void pack_send_robotData(uint16_t _data_cmd_id, uint16_t _receiver_ID, uint8_t* _data, uint16_t _data_len);	//0x301交互数据，包括UI绘制和车间通信
        void send_toReferee(uint16_t _com_id, uint16_t length, receive_Type_e _receive_type);




#endif