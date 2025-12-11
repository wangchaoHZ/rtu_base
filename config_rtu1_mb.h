/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors: wangchao
 * @LastEditTime: 2025-12-11 08:42:48
 * @FilePath: \JD_SCC80\MB_RTU_MTR\config_rtu1_mb.h
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#ifndef _CONFIG_RTU1_MB_H
#define _CONFIG_RTU1_MB_H
/*
 *  RTTHREAD PORT
 */
#define RTU1_MODBUS_RTTHREAD            1u
#define RTU1_MODBUS_RTTHREAD_USE_DEVICE 1u
#if RTU1_MODBUS_RTTHREAD
#include "rtu1_mb_port_rtthread.h"
#endif

#define RTU1_MODBUS_CRC_BYTE_SWAP 0u

/*
 *  USER CONFIG
 *  A3+:CN3-7Pin
 *  B3-:CN3-8Pin
 */
#define RTU1_SLAVE_ADDR        1u
#define RTU1_UART_DEVICE_NAME  "uart3"
#define RTU1_THREAD_STACK_SIZE 1024u
#define RTU1_THREAD_PRIORITY   19u
#define RTU1_THREAD_TICKS      15u
#endif
/* __CONFIG_RTU1_H__ */
