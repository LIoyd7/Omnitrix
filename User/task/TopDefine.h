#ifndef TOP_DEFINE_H
#define TOP_DEFINE_H

//===================用户配置===================
// 是否使用freertos
#ifndef _FREERTOS
#define _FREERTOS 1
#endif
// 是否开启调试
#ifndef DEBUG
#define DEBUG 1
#endif
// 是否使用自动
#ifndef _AUTO
#define _AUTO 0
#endif

#define ONE_CONTROL 0

// 是否使用大疆DT7遥控器
#ifndef DT7
#define DT7 0
#endif

//=============================================
//================任务通知,时间组================//
// 事件组
#define EVENT_RC (1 << 1)
#define EVENT_CAN1 (1 << 2)
#define EVENT_CAN2 (1 << 3)

//================任务通知================//
// 运球
#define BALL_DOWN (1 << 1)

// 可以蓄力标志
#define READY_TELL (1 << 0)
// 运球完成
#define HANDING_FINISH (1 << 2)
// 伸缩结束
#define EXTEND_OK (1 << 3)
// 等待ok
#define WAIT_OK (1 << 4)
// 可以防守收回
#define DEF_READY (1 << 5)

// 要发送ok了
#define BALL_SEND (1 << 6)
// 能够处理放球命令
#define PUT_ENABLE (1 << 7)

// userTask里定义机器人状态
typedef enum
{
    ROBOT_IN_ONE = 1,
    // ROBOT_IN_TWO,
    ROBOT_FIND_BALL,
    ROBOT_PUT_BALL,
    ROBOT_ERROR
} robot_status_e;

#endif
//================任务通知,时间组================//