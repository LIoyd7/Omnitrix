#include "TopDefine.h"
#include "FreeRTOS.h"
#include "userTask.h"
#include <cmsis_os2.h>
#include "main.h"

int a=0;

void Function_led(void *argument)
{
    (void)argument; /* 未使用argument，消除警告 */

    const uint32_t delay_tick = osKernelGetTickFreq() / TASK_FREQ_LED;

    uint32_t tick = osKernelGetTickCount();

    while (1)
    {
#ifdef DEBUG
        task_struct.stack_water_mark.led = osThreadGetStackSpace(osThreadGetId());
#endif

        // HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
         HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
        //HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
        // HAL_GPIO_TogglePin(BLE_EN_GPIO_Port, BLE_EN_Pin);
        a++;
        osDelay(500);

        tick += delay_tick; /* 计算下一个唤醒时刻 */
        osDelayUntil(tick);
    }
}
