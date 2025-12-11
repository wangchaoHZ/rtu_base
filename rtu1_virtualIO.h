/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors: wangchao
 * @LastEditTime: 2025-06-09 09:26:15
 * @FilePath: \STD_BMS_MB_FW\MB_RTU_MTR\rtu1_virtualIO.h
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */

#ifndef _RTU1_VIRTUALIO_H
#define _RTU1_VIRTUALIO_H

#include "rtu1_mb.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

int rtu1_read_hold_regs(uint16_t addr, uint16_t num, uint16_t* buffer);
int rtu1_write_hold_regs(uint16_t addr, uint16_t num, uint16_t* buffer);
int rtu1_read_hold_coils(uint16_t addr, uint16_t num, uint8_t* buffer);
int rtu1_read_input_coils(uint16_t addr, uint16_t num, uint8_t* buffer);
int rtu1_read_input_regs(uint16_t addr, uint16_t num, uint16_t* buffer);
int rtu1_write_hold_coils(uint16_t addr, uint16_t num, uint8_t* buffer);

#endif
