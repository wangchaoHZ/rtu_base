/*
 * @Author        : wang chao
 * @Date          : 2024-03-27 14:48:38
 * @LastEditors   : wang chao
 * @LastEditTime  : 2024-03-27 15:43:36
 * @FilePath      : \rtu1_mb_port_rtthread.h
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#ifndef _RTU1_MB_PORT_RTTHREAD_H
#define _RTU1_MB_PORT_RTTHREAD_H

#include "config_rtu1_mb.h"
#include "rtu1_mb_base.h"
#include "stdint.h"
#include "string.h"

/*
 * mb on rtthread
 */
#if RTU1_MODBUS_RTTHREAD
/*
 *rtu1_mb_init
 */
int rtu1_mb_init(rtu1_mb_t *smb, uint8_t core_type, void *port);
rtu1_mb_t *rtu1_mb_create(uint8_t core_type, void *port);

/*
 *mb port rtthread device
 */
#if RTU1_MODBUS_RTTHREAD_USE_DEVICE

#include <rtdevice.h>
#include <rtthread.h>

typedef struct rtu1_mb_port_rtdevice rtu1_mb_port_rtdevice_t;

struct rtu1_mb_port_rtdevice {
    struct _rtu1_mb_port base;
    struct serial_configure serial_config;
    const char *device_name;
    struct rt_device *device;
    struct rt_semaphore rx_sem;
    rt_size_t rx_size;
    int oflag;
    int (*rts_set)(int on);
};

rtu1_mb_port_rtdevice_t *rtu1_mb_port_rtdevice_get(rtu1_mb_t *smb);

int rtu1_mb_port_rtdevice_init(rtu1_mb_port_rtdevice_t *port,
                                   const char *device_name);
int rtu1_mb_port_rtdevice_deinit(rtu1_mb_port_rtdevice_t *port);
rtu1_mb_port_rtdevice_t *
rtu1_mb_port_rtdevice_create(const char *device_name);
int rtu1_mb_port_rtdevice_delete(void *port);
int rtu1_mb_rtu_set_serial_rts(rtu1_mb_t *smb, int (*rts_set)(int on));
int rtu1_mb_rtu_set_serial_name(rtu1_mb_t *smb,
                                    const char *device_name);
int rtu1_mb_rtu_set_serial_config(rtu1_mb_t *smb,
                                      struct serial_configure *serial_config);
int rtu1_mb_rtu_set_oflag(rtu1_mb_t *smb, int oflag);

#endif
#endif
#endif
