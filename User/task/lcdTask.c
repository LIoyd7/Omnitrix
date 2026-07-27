#include "TopDefine.h"
#include "FreeRTOS.h"
#include "userTask.h"
#include <cmsis_os2.h>
#include "main.h"
#include "device/lcd_init.h"
#include "device/lcd.h"

// 全局计数，用于示例显示
int lcd_counter = 0;

void Function_lcd(void *argument)
{
    (void)argument; /* 未使用 argument，消除警告 */

    const uint32_t delay_tick = osKernelGetTickFreq() / TASK_FREQ_LCD;

    uint32_t tick = osKernelGetTickCount();

    // LCD 初始化
    LCD_Init();

    // 开启背光 PWM 并设置亮度 80%
    LCD_Open_Light();
    LCD_Set_Light(80);

    // 全屏点亮白色
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);

    while (1)
    {
#ifdef DEBUG
        task_struct.stack_water_mark.lcd = osThreadGetStackSpace(osThreadGetId());
#endif

        lcd_counter++;

        osDelay(500);

        tick += delay_tick;
        osDelayUntil(tick);
    }
}
