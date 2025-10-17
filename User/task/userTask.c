#include "userTask.h"

task_t task_struct; /* 任务结构体实例化 */

/*下面是所有的任务属性*/

const osThreadAttr_t attr_led = {
    .name = "led",
    .priority = osPriorityLow,
    .stack_size = 128 * 4,
};

const osThreadAttr_t attr_lcd = {
    .name = "lcd",
    .priority = osPriorityLow,
    .stack_size = 256 * 4,
};

const osThreadAttr_t attr_comm = {
    .name = "comm",
    .priority = osPriorityLow,
    .stack_size = 128 * 4,
};

const osEventFlagsAttr_t attr_event = {
    .name = "event",
};
