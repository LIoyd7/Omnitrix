/**
 * 初始化任务
 */
#include "TopDefine.h"
#include "userTask.h"


/*********************/
/**
 * \brief 初始化
 *
 * \param argument 未使用
 */
void initFunction(void *argument)
{
    (void)argument; /* 未使用argument，消除警告 */

    osKernelLock();
    /* 创建任务 */

    task_struct.thread.led =
        osThreadNew(Function_led, NULL, &attr_led);
    /* 创建消息队列 */
    // task_struct.msgq.msg =
    //     osMessageQueueNew(2u, sizeof(CAN_CapOutput_t), NULL);


    /*创建事件组*/
    task_struct.events.eventReceive = osEventFlagsNew(&attr_event);

    osKernelUnlock();
    osThreadTerminate(osThreadGetId()); /* 结束自身 */
}
