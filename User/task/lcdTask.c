#include "TopDefine.h"
#include "FreeRTOS.h"
#include "userTask.h"
#include <cmsis_os2.h>
#include "main.h"
#include "device/lcd_init.h" // LCD 初始化头文件

// 全局计数，用于示例显示
int lcd_counter = 0;

void Function_lcd(void *argument)
{
    (void)argument; /* 未使用 argument，消除警告 */

    const uint32_t delay_tick = osKernelGetTickFreq() / TASK_FREQ_LCD; // TASK_FREQ_LCD 定义刷新频率

    uint32_t tick = osKernelGetTickCount();

    // LCD 初始化
    LCD_Init();
    LCD_Set_Light(80); // 背光 80%

    while (1)
    {
#ifdef DEBUG
        task_struct.stack_water_mark.lcd = osThreadGetStackSpace(osThreadGetId());
#endif

        // 示例: 每次循环在 LCD 上显示一个计数
        // 这里你可以调用你的绘图函数，比如 LCD_Fill / LCD_DrawString / LCD_DrawPixel 等
        lcd_counter++;
        // LCD_Fill(0xFFFF); // 全屏白色
        // LCD_ShowNumber(10, 10, lcd_counter, 3, 0x001F); // 假设你有这个函数

        osDelay(500); // 先简单延时 500ms

        tick += delay_tick; /* 计算下一个唤醒时刻 */
        osDelayUntil(tick);
    }
}
