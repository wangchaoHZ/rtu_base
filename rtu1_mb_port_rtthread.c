/*
 * Change Logs:
 * Date           Author       Notes
 * 2021-03          chenbin      rtu_mb_rtthread.c  for rtthread
 */

#include "rtu1_mb_port_rtthread.h"
/*
 * mb on rtthread
 */
#if RTU1_MODBUS_RTTHREAD
#include "rtu1_mb_base.h"
#include "rtu1_mb_core.h"
#include "rtu1_mb_utils.h"

int _rtu1_mb_debug(rtu1_mb_t *smb, int level, const char *fmt, ...) {
    static char log_buf[256];
    rt_enter_critical();
    if (level <= smb->debug_level) {
        va_list args;
        va_start(args, fmt);
        rt_vsnprintf(log_buf, 256, fmt, args);
        va_end(args);
        rt_kputs(log_buf);
    }
    rt_exit_critical();
    return 0;
}
/*
 *rtu1_mb_init
 */
int rtu1_mb_init(rtu1_mb_t *smb, uint8_t core_type, void *port) {
    rtu1_mb_port_t *smb_port = port;
    if (smb && core_type && smb_port) {
        _rtu1_mb_init(smb);
        if ((core_type == MODBUS_CORE_RTU))// check core type
        {
            smb->core = (rtu1_mb_core_t *) &_rtu1_mb_core;
        } else {
            return 0;
        }
        if ((smb_port->type == MODBUS_PORT_DEVICE) ||
            (smb_port->type == MODBUS_PORT_SOCKET)) {
            smb->port = smb_port;
            return 1;
        }
    }
    return 0;
}

rtu1_mb_t *rtu1_mb_create(uint8_t core_type, void *port) {
    rtu1_mb_t *smb = rt_malloc_align(sizeof(rtu1_mb_t), 4);
    if (smb) {
        if (rtu1_mb_init(smb, core_type, port)) {
            return smb;
        } else {
            rt_free_align(smb);
        }
    }
    return NULL;
}

/*
 *mb port device
 */
#ifdef RTU1_MODBUS_RTTHREAD_USE_DEVICE

#include <rtdevice.h>
#include <rtthread.h>

static rt_err_t _rtu1_mb_rtdevice_rx_indicate(rt_device_t dev, rt_size_t size) {
    rtu1_mb_port_rtdevice_t *smb_port_device = dev->user_data;

    smb_port_device->rx_size = size;

    return rt_sem_release(&(smb_port_device->rx_sem));
}

static int _rtu1_mb_rtdevice_open(rtu1_mb_t *smb) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            (rtu1_mb_port_rtdevice_t *) smb->port;
    if (smb_port_device->device) {
        smb_port_device->device->user_data = smb_port_device;

        rt_device_set_rx_indicate(smb_port_device->device,
                                  _rtu1_mb_rtdevice_rx_indicate);

        rt_device_open(smb_port_device->device, smb_port_device->oflag);

        if (smb_port_device->rts_set) {
            smb_port_device->rts_set(0);
        }
    }
    return 0;
}

static int _rtu1_mb_rtdevice_close(rtu1_mb_t *smb) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            (rtu1_mb_port_rtdevice_t *) smb->port;
    if (smb_port_device->device) {
        rt_device_close(smb_port_device->device);
    }
    return 0;
}

static int _rtu1_mb_rtdevice_write(rtu1_mb_t *smb, uint8_t *data,
                               uint16_t length) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            (rtu1_mb_port_rtdevice_t *) smb->port;

    if (smb_port_device->rts_set)
        smb_port_device->rts_set(1);

    rt_enter_critical();// poll write start

    rt_device_write(smb_port_device->device, 0, data, length);

    rt_exit_critical();// poll write end

    if (smb_port_device->rts_set)
        smb_port_device->rts_set(0);

    return length;
}

static int _rtu1_mb_rtdevice_read(rtu1_mb_t *smb, uint8_t *data,
                              uint16_t length) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            (rtu1_mb_port_rtdevice_t *) smb->port;

    return rt_device_read(smb_port_device->device, 0, data, length);
}

static int _rtu1_mb_rtdevice_flush(rtu1_mb_t *smb) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            (rtu1_mb_port_rtdevice_t *) smb->port;

    int rc = rt_device_read(smb_port_device->device, 0, smb->read_buff,
                            RTU1_MAX_ADU_LENGTH);

    rt_sem_control(&(smb_port_device->rx_sem), RT_IPC_CMD_RESET, RT_NULL);
    return rc;
}

static int _rtu1_mb_rtdevice_wait(rtu1_mb_t *smb, int timeout) {
    int rc = -1;
    rtu1_mb_port_rtdevice_t *smb_port_device =
            (rtu1_mb_port_rtdevice_t *) smb->port;

    rc = rt_sem_take(&(smb_port_device->rx_sem), timeout);
    if (rc < RT_EOK) {
        return MODBUS_TIMEOUT;
    }
    if (smb_port_device->rx_size == 0) {
        return MODBUS_ERROR_READ;
    }
    return rc;
}

rtu1_mb_port_rtdevice_t *rtu1_mb_port_rtdevice_get(rtu1_mb_t *smb) {
    if ((smb->port->type == MODBUS_PORT_DEVICE) &&
        (smb->port->magic == MODBUS_PORT_MAGIC)) {
        return (rtu1_mb_port_rtdevice_t *) smb->port;
    }
    return NULL;
}

int rtu1_mb_port_rtdevice_init(rtu1_mb_port_rtdevice_t *port,
                                   const char *device_name) {
    (*(uint32_t *) &(port->base.type)) = MODBUS_PORT_DEVICE;
    (*(uint32_t *) &(port->base.magic)) = MODBUS_PORT_MAGIC;
    port->base.open = _rtu1_mb_rtdevice_open;
    port->base.close = _rtu1_mb_rtdevice_close;
    port->base.read = _rtu1_mb_rtdevice_read;
    port->base.write = _rtu1_mb_rtdevice_write;
    port->base.flush = _rtu1_mb_rtdevice_flush;
    port->base.wait = _rtu1_mb_rtdevice_wait;

    port->device_name = device_name;
    port->device = rt_device_find(device_name);
    if (port->device) {
        rt_sem_init(&(port->rx_sem), device_name, 0, RT_IPC_FLAG_FIFO);
    }
    return 0;
}

int rtu1_mb_port_rtdevice_deinit(rtu1_mb_port_rtdevice_t *port) {
    if (port->device) {
        rt_device_close(port->device);
    }
    return 0;
}

rtu1_mb_port_rtdevice_t *
rtu1_mb_port_rtdevice_create(const char *device_name) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            rt_malloc_align(sizeof(rtu1_mb_port_rtdevice_t), 4);
    if (smb_port_device) {
        rt_memset(smb_port_device, 0, sizeof(rtu1_mb_port_rtdevice_t));
        rtu1_mb_port_rtdevice_init(smb_port_device, device_name);
        return smb_port_device;
    }
    return NULL;
}

int rtu1_mb_port_rtdevice_delete(void *port) {
    rtu1_mb_port_rtdevice_t *port_device = port;
    if (port_device && (port_device->base.type == MODBUS_PORT_DEVICE) &&
        (port_device->base.magic == MODBUS_PORT_MAGIC)) {
        rtu1_mb_port_rtdevice_deinit(port_device);
        rt_free_align(port_device);
    }
    return 0;
}

int rtu1_mb_rtu_set_serial_rts(rtu1_mb_t *smb, int (*rts_set)(int on)) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            rtu1_mb_port_rtdevice_get(smb);
    if (smb_port_device) {
        smb_port_device->rts_set = rts_set;
    }
    return 0;
}

int rtu1_mb_rtu_set_serial_name(rtu1_mb_t *smb,
                                    const char *device_name) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            rtu1_mb_port_rtdevice_get(smb);
    if (smb_port_device) {
        rtu1_mb_port_rtdevice_init(smb_port_device, device_name);
    }
    return 0;
}

int rtu1_mb_rtu_set_serial_config(rtu1_mb_t *smb,
                                      struct serial_configure *serial_config) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            rtu1_mb_port_rtdevice_get(smb);
    if (smb_port_device) {
        if (smb_port_device->device) {
            rt_device_control(smb_port_device->device, RT_DEVICE_CTRL_CONFIG,
                              serial_config);
        }
    }
    return 0;
}
int rtu1_mb_rtu_set_oflag(rtu1_mb_t *smb, int oflag) {
    rtu1_mb_port_rtdevice_t *smb_port_device =
            rtu1_mb_port_rtdevice_get(smb);
    if (smb_port_device) {
        smb_port_device->oflag = oflag;
    }
    return 0;
}

#endif
#endif
