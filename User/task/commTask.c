#include "TopDefine.h"
#include "FreeRTOS.h"
#include "userTask.h"
#include <cmsis_os2.h>
#include "main.h"
#include "device/watch_comm.h"

int a2[3] = {1, 2, 3};
int a1 = 0;

void Function_comm(void *argument)
{
    (void)argument; /* 未使用argument，消除警告 */

    const uint32_t delay_tick = osKernelGetTickFreq() / TASK_FREQ_COMM;
    uint32_t tick = osKernelGetTickCount();

    WatchFrame_t frame;
  

    while (1)
    {
#ifdef DEBUG
        task_struct.stack_water_mark.comm = osThreadGetStackSpace(osThreadGetId());
#endif

        a1++;
        WCOMM_Send(WCOMM_ACK, &a2, sizeof(&a2));
        /* 等待数据包就绪 */
        if (osThreadFlagsWait(0x01, osFlagsWaitAny, 20) == 0x01)
        {
            if (WCOMM_Parse(&frame) == 0)
            {
                switch (frame.type)
                {
                case WCOMM_HEARTBEAT:
                    // 收到心跳包，回应一个 ACK
                  //  WCOMM_Send(WCOMM_ACK, NULL, 0);
                    break;

                case WCOMM_SENSOR:
                    // 处理 PC 发来的传感器数据（如时间同步）
                    break;

                case WCOMM_CONTROL:
                    // 执行上位机发来的控制命令
                    break;

                default:
                    break;
                }
            }
            /* 继续接收下一帧 */
            WCOMM_StartReceiving();
        }

        tick += delay_tick; /* 计算下一个唤醒时刻 */
        osDelayUntil(tick);
    }
}
