#ifndef BSP_H
#define BSP_H

typedef enum
{
    BSP_OK = 0,
    BSP_ERR = -1,
    BSP_ERR_NULL = -2,
    BSP_ERR_INITED = -3,
    BSP_ERR_NO_DEV = -4
} bsp_status_e;

#endif
