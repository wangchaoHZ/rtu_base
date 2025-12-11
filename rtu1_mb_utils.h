/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors: wangchao
 * @LastEditTime: 2025-06-09 12:43:52
 * @FilePath: \STD_BMS_MB_FW\MB_RTU_MTR\rtu1_mb_utils.h
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#ifndef _RTU1_UTILS_H_
#define _RTU1_UTILS_H_

#include "stdint.h"
#include "stdio.h"
#include "string.h"

uint16_t rtu1_mb_crc16(uint8_t* buffer, uint16_t buffer_length);
int rtu1_mb_check_addr_num(uint8_t function, uint16_t address, uint16_t num);
void rtu1_mb_byte_copy(uint8_t* des, uint8_t* src, int num);
void rtu1_mb_coil_h2m(uint8_t* des, uint8_t* src, int coil_num);
void rtu1_mb_coil_m2h(uint8_t* des, uint8_t* src, int coil_num);
void rtu1_mb_reg_h2m(void* dest_mb_reg, void* source_host, int reg_num);
void rtu1_mb_reg_m2h(void* dest_host, void* source_mb_reg, int reg_num);

int rtu1_dio_get_val(uint8_t* array, uint16_t index);
void rtu1_dio_set_val(uint8_t* array, uint16_t index, uint16_t status);
uint16_t rtu1_aio_get_val(uint16_t* array, uint16_t index);
void rtu1_aio_set_val(uint16_t* array, uint16_t index, uint16_t status);

#endif
