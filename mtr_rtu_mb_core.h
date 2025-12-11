/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 16:08:47
 * @LastEditors   : wang chao
 * @LastEditTime  : 2024-03-27 16:11:12
 * @FilePath      : \modbus_rtu_mtr\mtr_rtu_mb_core.h
 * @Description   : 
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved. 
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-21     chenbin      small mb the first version
 */
#ifndef MTR_RTU_MB_CORE_H
#define MTR_RTU_MB_CORE_H

#include "mtr_rtu_mb_base.h"
extern const mtr_rtu_mb_core_t _mtr_rtu_mb_core;

#define _MTR_MODBUS_RTU_HEADER_LENGTH 1
#define _MTR_MODBUS_RTU_PRESET_REQ_LENGTH 6
#define _MTR_MODBUS_RTU_PRESET_RSP_LENGTH 2

#define _MTR_MODBUS_RTU_CHECKSUM_LENGTH 2
#define _MTR_MODBUS_RTU_MAX_ADU_LENGTH 260

#endif /* MTR_RTU_MB_CORE_H */
