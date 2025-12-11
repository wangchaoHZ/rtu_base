/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors: wangchao
 * @LastEditTime: 2025-09-23 18:00:29
 * @FilePath: \STD_BMS_MB_FW\MB_RTU_MTR\mtr_rtu_virtualIO.c
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#include "mtr_rtu_virtualIO.h"
#include "board.h"
#include "mtr_rtu_mb.h"
#include "stdio.h"
#include "string.h"

#if 0

#define DBG_TAG "MTR"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define HOLD_REG_MAX 125
static uint16_t mtr_hold_regs[HOLD_REG_MAX] = {0};

int mtr_rtu_read_hold_regs(uint16_t addr, uint16_t num, uint16_t* buffer)
{
    LOG_I("Read Reg Addr:%d Num:%d", addr, num);

    if (addr >= HOLD_REG_MAX)
    {
        return 0;
    }

    uint16_t valid_num = ((addr + num) > HOLD_REG_MAX) ? (HOLD_REG_MAX - addr) : num;

    memcpy(buffer, &mtr_hold_regs[addr], valid_num * sizeof(uint16_t));

    return valid_num;
}

int mtr_rtu_write_hold_regs(uint16_t addr, uint16_t num, uint16_t* buffer)
{
    if (addr >= HOLD_REG_MAX)
    {
        return 0;
    }

    LOG_I("Write Reg Addr:%d Num:%d", addr, num);

    uint16_t valid_num = ((addr + num) > HOLD_REG_MAX) ? (HOLD_REG_MAX - addr) : num;

    memcpy(&mtr_hold_regs[addr], buffer, valid_num * sizeof(uint16_t));

    return valid_num;
}

#define INPUT_REG_MAX 125
uint16_t mtr_input_regs[INPUT_REG_MAX] = {0};

int mtr_rtu_read_input_regs(uint16_t addr, uint16_t num, uint16_t* buffer)
{
    LOG_I("Read Input Reg Addr:%d Num:%d", addr, num);

    if (addr >= INPUT_REG_MAX)
        return 0;

    uint16_t valid_num = (addr + num > INPUT_REG_MAX) ? (INPUT_REG_MAX - addr) : num;

    memcpy(buffer, &mtr_input_regs[addr], valid_num * sizeof(uint16_t));

    return valid_num;
}

#define HOLD_COILS_MAX 16
uint8_t mtr_hold_coils[HOLD_COILS_MAX / 8] = {0};

int mtr_rtu_read_hold_coils(uint16_t addr, uint16_t num, uint8_t* buffer)
{
    if (addr >= HOLD_COILS_MAX)
    {
        return 0;
    }

    uint16_t valid_num = ((addr + num) > HOLD_COILS_MAX) ? (HOLD_COILS_MAX - addr) : num;

    uint16_t byte_len = (valid_num + 7) / 8;
    memset(buffer, 0, byte_len);

    for (uint16_t i = 0; i < valid_num; ++i)
    {
        uint16_t src_bit_index = addr + i;
        uint8_t bit_val = (mtr_hold_coils[src_bit_index >> 3] >> (src_bit_index & 0x07)) & 0x01;

        // 将 bit 写入目标 buffer 的对应 bit 位
        buffer[i >> 3] |= (bit_val << (i & 0x07));
    }

    return valid_num;
}

#define INPUT_COILS_MAX 16
uint8_t mtr_input_coils[INPUT_COILS_MAX / 8] = {0};

int mtr_rtu_read_input_coils(uint16_t addr, uint16_t num, uint8_t* buffer)
{
    if (addr >= INPUT_COILS_MAX || buffer == NULL)
    {
        return 0;
    }

    uint16_t valid_num = ((addr + num) > INPUT_COILS_MAX) ? (INPUT_COILS_MAX - addr) : num;

    uint16_t byte_len = (valid_num + 7) / 8;
    memset(buffer, 0, byte_len);

    for (uint16_t i = 0; i < valid_num; ++i)
    {
        uint16_t src_bit_index = addr + i;
        uint8_t bit_val = (mtr_input_coils[src_bit_index >> 3] >> (src_bit_index & 0x07)) & 0x01;

        buffer[i >> 3] |= (bit_val << (i & 0x07));
    }

    return valid_num;
}

int mtr_rtu_write_hold_coils(uint16_t addr, uint16_t num, uint8_t* buffer)
{
    if (addr >= HOLD_COILS_MAX)
        return 0;

    uint16_t valid_num = (addr + num > HOLD_COILS_MAX) ? (HOLD_COILS_MAX - addr) : num;

    uint16_t byte_start = addr / 8;
    uint16_t bit_offset = addr % 8;
    uint16_t byte_end = (addr + valid_num + 7) / 8;

    // 将 buffer 中的位逐个复制到 mtr_hold_coils 中，考虑位偏移
    for (uint16_t i = 0; i < valid_num; ++i)
    {
        uint16_t bit_index = addr + i;
        int val_new = mtr_rtu_dio_get_val(buffer, i);
        mtr_rtu_dio_set_val(mtr_hold_coils, bit_index, val_new);
    }

    return valid_num;
}

#endif