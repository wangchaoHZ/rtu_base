/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors   : wang chao
 * @LastEditTime  : 2024-03-27 16:10:33
 * @FilePath      : \modbus_rtu_mtr\rtu1_mb_rtu.c
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-21     chenbin      small mb the first version
 */
#include "rtu1_mb_core.h"
#include "config_rtu1_mb.h"
#include "rtu1_mb_utils.h"

/* Builds a RTU request header */
static int rtu1_build_request_header(rtu1_mb_t *smb, uint8_t *buff,
                                         int slave, int fun, int reg, int num) {
    buff[0] = slave;
    buff[1] = fun;
    buff[2] = reg >> 8;
    buff[3] = reg & 0x00ff;
    buff[4] = num >> 8;
    buff[5] = num & 0x00ff;
    return _RTU1_RTU_PRESET_REQ_LENGTH;
}

/* Builds a RTU response header */
static int rtu1_build_response_header(rtu1_mb_t *smb, uint8_t *buff,
                                          int slave, int fun) {
    buff[0] = slave;
    buff[1] = fun;
    return _RTU1_RTU_PRESET_RSP_LENGTH;
}

static int rtu1_check_send_pre(rtu1_mb_t *smb, uint8_t *buff,
                                   int length) {
    uint16_t crc = rtu1_mb_crc16(buff, length);
    buff[length++] = crc & 0x00FF;// LOW BYTE
    buff[length++] = crc >> 8;    // HIGH BYTE
    return length;
}

static int rtu1_check_wait_request(rtu1_mb_t *smb, uint8_t *buff,
                                       int length) {
    uint16_t crc_recv = ((uint16_t) (buff[length - 2] & 0x00FF) +
                         (uint16_t) (buff[length - 1] << 8));// LOW BYTE HIGH BYTE
    uint16_t crc_cal = rtu1_mb_crc16(buff, length - 2);
    if (crc_cal != crc_recv) {
        rtu1_mb_debug_error(smb, "crc  0x%04X != 0x%04X\n", crc_cal, crc_recv);
        return MODBUS_FAIL_CHECK;
    }
    if (buff[0] != smb->slave_addr && buff[0] != RTU1_BROADCAST_ADDRESS) {
        rtu1_mb_debug_error(smb, "slave adrr: 0x%02X != 0x%02X\n", buff[0],
                                smb->slave_addr);
        return MODBUS_FAIL_ADRR;
    }
    return length;
}

static int rtu1_check_wait_response(rtu1_mb_t *smb, uint8_t *buff,
                                        int length) {
    uint16_t crc_recv = ((uint16_t) (buff[length - 2] & 0x00FF) +
                         (uint16_t) (buff[length - 1] << 8));// LOW BYTE HIGH BYTE
    uint16_t crc_cal = rtu1_mb_crc16(buff, length - 2);
    if (crc_cal != crc_recv) {
        rtu1_mb_debug_error(smb, "crc  0x%04X != 0x%04X\n", crc_cal, crc_recv);
        return MODBUS_FAIL_CHECK;
    }
    if (buff[0] != smb->slave_addr && buff[0] != RTU1_BROADCAST_ADDRESS) {
        rtu1_mb_debug_error(smb, "slave adrr: 0x%02X != 0x%02X\n", buff[0],
                                smb->slave_addr);
        return MODBUS_FAIL_ADRR;
    }
    return length;
}

const rtu1_mb_core_t _rtu1_mb_core = {
        .magic = MODBUS_CORE_MAGIC,
        .type = MODBUS_CORE_RTU,
        .len_header = _RTU1_RTU_HEADER_LENGTH,
        .len_checksum = _RTU1_RTU_CHECKSUM_LENGTH,
        .len_adu_max = _RTU1_RTU_MAX_ADU_LENGTH,
        .build_request_header = rtu1_build_request_header,
        .build_response_header = rtu1_build_response_header,
        .check_send_pre = rtu1_check_send_pre,
        .check_wait_request = rtu1_check_wait_request,
        .check_wait_response = rtu1_check_wait_response};
