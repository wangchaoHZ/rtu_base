/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors: wangchao
 * @LastEditTime: 2025-08-22 07:38:34
 * @FilePath: \STD_BMS_MB_FW\MB_RTU_MTR\config_mtr_rtu_mb.h
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#ifndef _CONFIG_MTR_RTU_MB_H
#define _CONFIG_MTR_RTU_MB_H
/*
 *  RTTHREAD PORT
 */
#define MTR_RTU_MODBUS_RTTHREAD            1u
#define MTR_RTU_MODBUS_RTTHREAD_USE_DEVICE 1u
#if MTR_RTU_MODBUS_RTTHREAD
#include "mtr_rtu_mb_port_rtthread.h"
#endif

#define MTR_RTU_MODBUS_CRC_BYTE_SWAP 0u

/*
 *  USER CONFIG
 *  A3+:CN3-7Pin
 *  B3-:CN3-8Pin
 */
#define MTR_RTU_SLAVE_ADDR        1u
#define MTR_RTU_UART_DEVICE_NAME  "uart4"
#define MTR_RTU_THREAD_STACK_SIZE 1024u
#define MTR_RTU_THREAD_PRIORITY   19u
#define MTR_RTU_THREAD_TICKS      15u
#endif
/* __CONFIG_MTR_MODBUS_H__ */
