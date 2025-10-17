#include "watch_comm.h"
#include <string.h>

static uint8_t rx_buf[64]; // DMA接收缓存
static osThreadId_t comm_thread;

static void UART_RxCpltCallback(void)
{
    osThreadFlagsSet(comm_thread, 0x01);
}

int8_t WCOMM_Init(void)
{
    comm_thread = osThreadGetId();
    BSP_UART_RegisterCallback(BSP_UART_TYPEC, BSP_UART_RX_CPLT_CB, UART_RxCpltCallback);
    return WCOMM_StartReceiving();
}

int8_t WCOMM_StartReceiving(void)
{
    return (HAL_UART_Receive_DMA(BSP_UART_GetHandle(BSP_UART_TYPEC), rx_buf, sizeof(rx_buf)) == HAL_OK) ? 0 : -1;
}

// 发送一帧
int8_t WCOMM_Send(uint8_t type, void *data, uint8_t len)
{
    uint8_t tx_buf[64];
    if (len > 32)
        return -1;

    tx_buf[0] = WCOMM_HEAD;
    tx_buf[1] = type;
    tx_buf[2] = len;
    memcpy(&tx_buf[3], data, len);

    // 简单校验和
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < len; i++)
        checksum += ((uint8_t *)data)[i];
    tx_buf[3 + len] = checksum;

    tx_buf[4 + len] = WCOMM_TAIL;

    return (HAL_UART_Transmit(BSP_UART_GetHandle(BSP_UART_TYPEC), tx_buf, len + 5, HAL_MAX_DELAY) == HAL_OK) ? 0 : -1;
}

// 解析一帧
int8_t WCOMM_Parse(WatchFrame_t *frame)
{
    if (rx_buf[0] != WCOMM_HEAD || rx_buf[rx_buf[2] + 4] != WCOMM_TAIL)
    {
        return -1;
    }

    frame->type = rx_buf[1];
    frame->len = rx_buf[2];
    memcpy(frame->data, &rx_buf[3], frame->len);

    // 校验和验证
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < frame->len; i++)
        checksum += frame->data[i];
    if (checksum != rx_buf[3 + frame->len])
        return -2;

    return 0;
}
