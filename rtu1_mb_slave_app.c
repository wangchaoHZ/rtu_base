/*
 * @Author        : wang chao
 * @Date          : 2024-03-22 15:23:08
 * @LastEditors: wangchao
 * @LastEditTime: 2025-12-11 08:43:28
 * @FilePath: \JD_SCC80\MB_RTU_MTR\rtu1_mb_slave_app.c
 * @Description   :
 * @Copyright     : Copyright 2024 wang chao, All Rights Reserved.
 */
#include "board.h"
#include "rtu1_mb.h"
#include "rtu1_virtualIO.h"
#include "stdio.h"
#include "string.h"

static int rtu1_rtu_slave_callback(rtu1_mb_t* smb, int function_code, int addr, int num, void* read_write_data)
{
    int rc = 0;

    switch (function_code)
    {
        case MODBUS_FC_READ_HOLDING_COILS:
        {
            // Function code 0x01 is used to read coils
            rc = rtu1_read_hold_coils(addr, num, read_write_data);
        }
        break;

        case MODBUS_FC_READ_INPUTS_COILS:
        {
            // Function code 0x02 is used to read input coils
            rc = rtu1_read_input_coils(addr, num, read_write_data);
        }
        break;

        case MODBUS_FC_READ_HOLDING_REGISTERS:
        {
            // Function code 0x03 is used to read holding registers
            if ((0 <= addr) && (addr < 125))
            {
                rc = rtu1_read_hold_regs(addr, num, read_write_data);
            }
        }
        break;

        case MODBUS_FC_READ_INPUT_REGISTERS:
        {
            // Function code 0x04 is used to read input registers
            if ((0 <= addr) && (addr < 125))
            {
                rc = rtu1_read_input_regs(addr, num, read_write_data);
            }
        }
        break;

        case MODBUS_FC_WRITE_SINGLE_COIL:
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        {
            // Function code 0x05 is used to write a single coil
            // Function code 0x0F is used to write multiple coils
            rc = rtu1_write_hold_coils(addr, num, read_write_data);
        }
        break;

        case MODBUS_FC_WRITE_SINGLE_REGISTER:
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        {
            // Function code 0x06 is used to write a single register
            // Function code 0x10 is used to write multiple registers
            if ((0 <= addr) && (addr < 125))
            {
                rc = rtu1_write_hold_regs(addr, num, read_write_data);
            }
        }

        break;
    }

    if (rc < 0)
    {
        rt_kprintf("Callback failed %d\n", rc);
    }
    return rc;
}

static int uart_rts(int on)
{
    return 0;
}

static rtu1_mb_t modbus_rtu_slave = {0};

static void rtu1_rtu_slave_thread_entry(void* param)
{
    int rc = 0;
    rtu1_mb_t* smb_slave = param;
    rtu1_mb_init(smb_slave, MODBUS_CORE_RTU, rtu1_mb_port_rtdevice_create(RTU1_UART_DEVICE_NAME));
    struct serial_configure serial_config;
    serial_config.baud_rate = BAUD_RATE_115200;
    serial_config.data_bits = DATA_BITS_8;
    serial_config.stop_bits = STOP_BITS_1;
    serial_config.bufsz = 512;
    serial_config.parity = PARITY_NONE;
    rtu1_mb_rtu_set_serial_config(smb_slave, &serial_config);
    rtu1_mb_rtu_set_serial_rts(smb_slave, uart_rts);
    rtu1_mb_rtu_set_oflag(smb_slave, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);

    rtu1_mb_set_slave(smb_slave, RTU1_SLAVE_ADDR);
    rtu1_mb_connect(smb_slave);

    for (;;)
    {
        rc = rtu1_mb_slave_wait_handle(smb_slave, rtu1_rtu_slave_callback, MODBUS_WAIT_FOREVER);
        if (rc > 0)
        {
            continue;
        }
        else
        {
            rtu1_mb_error_recovery(smb_slave);
        }
    }
}

int rtu1_rtu_slave_thread(void)
{
    rt_thread_t thread_id;

    thread_id = rt_thread_create("RTU1", rtu1_rtu_slave_thread_entry, &modbus_rtu_slave, RTU1_THREAD_STACK_SIZE, RTU1_THREAD_PRIORITY, RTU1_THREAD_TICKS);
    if (thread_id != RT_NULL)
    {
        rt_thread_startup(thread_id);
    }
    else
    {
        goto __exit;
    }

    return RT_EOK;
__exit:
    if (thread_id)
        rt_thread_delete(thread_id);
    return RT_ERROR;
}

#if 1

MSH_CMD_EXPORT(rtu1_rtu_slave_thread, "Start MTR Modbus RTU slave thread");

#endif