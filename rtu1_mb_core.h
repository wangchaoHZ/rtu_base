/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 16:08:47
 * @LastEditors   : wang chao
 * @LastEditTime  : 2024-03-27 16:11:12
 * @FilePath      : \modbus_rtu_mtr\rtu1_mb_core.h
 * @Description   : 
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved. 
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-21     chenbin      small mb the first version
 */
#ifndef _RTU1_MB_CORE_H
#define _RTU1_MB_CORE_H

#include "rtu1_mb_base.h"
extern const rtu1_mb_core_t _rtu1_mb_core;

#define _RTU1_RTU_HEADER_LENGTH 1
#define _RTU1_RTU_PRESET_REQ_LENGTH 6
#define _RTU1_RTU_PRESET_RSP_LENGTH 2

#define _RTU1_RTU_CHECKSUM_LENGTH 2
#define _RTU1_RTU_MAX_ADU_LENGTH 260

#endif /* _RTU1_RTU_H_ */
