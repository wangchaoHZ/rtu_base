/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors   : wang chao
 * @LastEditTime  : 2024-03-27 15:43:36
 * @FilePath      : \mtr_rtu_mb_port_rtthread.h
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#ifndef MTR_RTU_MB_PORT_RTTHREAD_H
#define MTR_RTU_MB_PORT_RTTHREAD_H

#include "config_mtr_rtu_mb.h"
#include "mtr_rtu_mb_base.h"
#include "stdint.h"
#include "string.h"

/*
 * mb on rtthread
 */
#if MTR_RTU_MODBUS_RTTHREAD
/*
 *mtr_rtu_mb_init
 */
int mtr_rtu_mb_init(mtr_rtu_mb_t *smb, uint8_t core_type, void *port);
mtr_rtu_mb_t *mtr_rtu_mb_create(uint8_t core_type, void *port);

/*
 *mb port rtthread device
 */
#if MTR_RTU_MODBUS_RTTHREAD_USE_DEVICE

#include <rtdevice.h>
#include <rtthread.h>

typedef struct mtr_rtu_mb_port_rtdevice mtr_rtu_mb_port_rtdevice_t;

struct mtr_rtu_mb_port_rtdevice {
    struct _mtr_rtu_mb_port base;
    struct serial_configure serial_config;
    const char *device_name;
    struct rt_device *device;
    struct rt_semaphore rx_sem;
    rt_size_t rx_size;
    int oflag;
    int (*rts_set)(int on);
};

mtr_rtu_mb_port_rtdevice_t *mtr_rtu_mb_port_rtdevice_get(mtr_rtu_mb_t *smb);

int mtr_rtu_mb_port_rtdevice_init(mtr_rtu_mb_port_rtdevice_t *port, const char *device_name);
int mtr_rtu_mb_port_rtdevice_deinit(mtr_rtu_mb_port_rtdevice_t *port);
mtr_rtu_mb_port_rtdevice_t *mtr_rtu_mb_port_rtdevice_create(const char *device_name);
int mtr_rtu_mb_port_rtdevice_delete(void *port);
int mtr_rtu_mb_rtu_set_serial_rts(mtr_rtu_mb_t *smb, int (*rts_set)(int on));
int mtr_rtu_mb_rtu_set_serial_name(mtr_rtu_mb_t *smb, const char *device_name);
int mtr_rtu_mb_rtu_set_serial_config(mtr_rtu_mb_t *smb, struct serial_configure *serial_config);
int mtr_rtu_mb_rtu_set_oflag(mtr_rtu_mb_t *smb, int oflag);

#endif /* MTR_RTU_MODBUS_RTTHREAD_USE_DEVICE */
#endif /* MTR_RTU_MODBUS_RTTHREAD */
#endif /* MTR_RTU_MB_PORT_RTTHREAD_H */
