#ifndef _WATCH_COMM_H_
#define _WATCH_COMM_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <cmsis_os2.h>
#include "bsp/struct_typedef.h"
#include "bsp/uart_it.h"
#include "main.h"

#define WCOMM_HEAD 0xAA
#define WCOMM_TAIL 0x55

// 数据类型定义
#define WCOMM_HEARTBEAT 0x01 // 心跳包
#define WCOMM_SENSOR 0x02    // 传感器数据
#define WCOMM_CONTROL 0x03   // 上位机下发控制指令
#define WCOMM_ACK 0x04       // ACK 确认

    typedef struct
    {
        uint8_t type;     // 数据类型
        uint8_t len;      // 数据长度
        uint8_t data[32]; // 实际内容（例如心率、步数、时间、命令等）
    } WatchFrame_t;

    int8_t WCOMM_Init(void);
    int8_t WCOMM_StartReceiving(void);
    int8_t WCOMM_Send(uint8_t type, void *data, uint8_t len);
    int8_t WCOMM_Parse(WatchFrame_t *frame);

#ifdef __cplusplus
}
#endif
#endif // _WATCH_COMM_H_
