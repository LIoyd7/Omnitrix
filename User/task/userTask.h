#ifndef USER_TASK_H
#define USER_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <cmsis_os2.h>
#include "FreeRTOS.h"
#include "task.h"

#include "TopDefine.h"
/* Exported constants ------------------------------------------------------- */

/* 所有任务都要define一个“任务运行频率”和“初始化延时” */
#define TASK_FREQ_SHOOT (500u)
#define TASK_FREQ_CAN (500u)
#define TASK_FREQ_AI (500u)
#define TASK_FREQ_LED (500u)

#define TASK_INIT_DELAY_INFO (500u)
#define TASK_INIT_DELAY_MONITOR (10)
#define TASK_INIT_DELAY_REFEREE (400u)
#define TASK_INIT_DELAY_AI (400u)

    /* Exported defines --------------------------------------------------------- */
    /* Exported macro ----------------------------------------------------------- */
    /* Exported types ----------------------------------------------------------- */

    typedef struct
    {
        /*线程任务*/
        struct
        {
            osThreadId_t led;

        } thread;

        /*消息队列*/
        struct
        {
            osMessageQueueId_t msg;
            osMessageQueueId_t status;
        } msgq;
        /*事件组*/
        struct
        {
            osEventFlagsId_t eventReceive;
        } events;

#ifdef DEBUG
        struct
        {

            UBaseType_t led;

        } stack_water_mark; /* stack使用 */

        struct
        {

            float led;
        } freq; /* 任务运行频率 */

        struct
        {

            float led;
        } last_up_time; /* 任务最近运行时 */
#endif

    } task_t;

    /* Exported functions ------------------------------------------------------- */
    extern task_t task_struct; /* 任务结构体实例化 */

    extern const osThreadAttr_t attr_led;
    extern const osEventFlagsAttr_t attr_event;

    /* Exported functions prototypes -------------------------------------------- */

    void Function_led(void *argument);

#ifdef __cplusplus
}
#endif

#endif
