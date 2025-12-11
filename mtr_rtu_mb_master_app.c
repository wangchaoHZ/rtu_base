/*
 * @Author: wangchao
 * @Date: 2025-04-27
 * @LastEditors: wangchao
 * @FilePath: \STD_BMS_MB_FW\MB_RTU_MTR\mtr_rtu_mb_master_app.c
 * @Description: VFD RTU Modbus主站管理
 */
#include "stdio.h"
#include "string.h"
#include "board.h"
#include "mtr_rtu_mb.h"
#include "mtr_rtu_virtualIO.h"

#define DBG_TAG "MTR_MASTER"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static mtr_rtu_mb_t mtr_rtu_master = {0};

static rt_bool_t mtr_rtu_master_startup = RT_FALSE;
static rt_mutex_t mtr_rtu_master_mux = RT_NULL;

#define TESTING_FUNCTION_CODE 0x10

// RTU主站回调函数
void mtr_rtu_master_poll(mtr_rtu_mb_t* rtu_master)
{
    int rc = 0, index = 0;
    uint8_t coils_state = 0;
    rt_int16_t regs_value = 0;

    // 设置从站地：1
    mtr_rtu_mb_set_slave(rtu_master, 1);

    LOG_I("RTU Master Polling started...");

    for (;;)
    {
#if (TESTING_FUNCTION_CODE == 0x01)
        /**
         *      示例：读取线圈/离散量输入状态
         *      功能码：0x01
         *      地址：0
         *      数量：8
         *      返回值：MODBUS_OK 或错误码
         */
        uint8_t temp_buff[16] = {0};

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_read_bits(rtu_master, 0, 8, temp_buff);  // modbus_read_input_bits
            LOG_I("Master Read 01 Coil Address 0x0000 Length 8: %d", rc);
            if (rc >= MODBUS_OK)
            {
                for (index = 0; index < 8; index++)
                {
                    rt_kprintf("[%d]", mtr_rtu_dio_get_val(temp_buff, index));
                }
                rt_kprintf("\n");
            }
        }

        rt_thread_mdelay(1000);

#endif

#if (TESTING_FUNCTION_CODE == 0x02)
        /**
         *      示例：读取离散量输入值
         *      功能码：0x02
         *      地址：0
         *      数量：8
         *      返回值：MODBUS_OK 或错误码
         */
        uint8_t temp_buff[16] = {0};

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_read_input_bits(rtu_master, 0, 8, temp_buff);  // modbus_read_bits
            LOG_I("Master Read 02 Coil Address 0x0000 Length 8: %d", rc);
            if (rc >= MODBUS_OK)
            {
                for (index = 0; index < 8; index++)
                {
                    rt_kprintf("[%d]", mtr_rtu_dio_get_val(temp_buff, index));
                }
                rt_kprintf("\n");
            }
        }

        rt_thread_mdelay(1000);
#endif

#if (TESTING_FUNCTION_CODE == 0x03)
        /**
         *      示例：读取保持寄存器值
         *      功能码：0x03
         *      地址：0
         *      数量：8
         *      返回值：MODBUS_OK 或错误码
         */

        uint16_t temp_buff[16] = {0};

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_read_registers(rtu_master, 0, 8, temp_buff);
            LOG_I("Master Read 03 Register Address 0x0000 Length 8: %d", rc);
            if (rc >= MODBUS_OK)
            {
                for (index = 0; index < 8; index++)
                {
                    rt_kprintf("[%d]", mtr_rtu_aio_get_val(temp_buff, index));
                }
                rt_kprintf("\n");
            }
        }

        rt_thread_mdelay(1000);
#endif

#if (TESTING_FUNCTION_CODE == 0x04)
        /**
         *      示例：读取保持寄存器值
         *      功能码：0x03
         *      地址：0
         *      数量：8
         *      返回值：MODBUS_OK 或错误码
         */

        uint16_t temp_buff[16] = {0};

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_read_input_registers(rtu_master, 0, 8, temp_buff);
            LOG_I("Master Read 04 Register Address 0x0000 Length 8: %d", rc);
            if (rc >= MODBUS_OK)
            {
                for (index = 0; index < 8; index++)
                {
                    rt_kprintf("[%d]", mtr_rtu_aio_get_val(temp_buff, index));
                }
                rt_kprintf("\n");
            }
        }

        rt_thread_mdelay(1000);
#endif

#if (TESTING_FUNCTION_CODE == 0x05)
        /**
         *      示例：写单个线圈或单个离散输出
         *      功能码：0x05
         *      地址：0
         *      写入值：0x01（开）或0x00（关）
         *      返回值：MODBUS_OK 或错误码
         */

        // 切换线圈状态
        coils_state = !coils_state;

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_write_bit(rtu_master, 0, coils_state);
            LOG_I("Master Write Coil Address 0x0000: %d", coils_state);
            if (rc >= MODBUS_OK)
            {
                LOG_I("Write bit success.");
            }
            else
            {
                LOG_E("Write bit failed: %d", rc);
            }
        }

        rt_thread_mdelay(1000);
#endif

#if (TESTING_FUNCTION_CODE == 0x06)
        /**
         *      示例：写单个线圈或单个离散输出
         *      功能码：0x05
         *      地址：0
         *      写入值：regs_value（0-100）
         *      返回值：MODBUS_OK 或错误码
         */

        regs_value = (regs_value >= 100) ? 0 : (regs_value + 1);

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_write_register(rtu_master, 0, regs_value);
            LOG_I("Master Write Register Address 0x0000: %d", regs_value);
            if (rc >= MODBUS_OK)
            {
                LOG_I("Write register success.");
            }
            else
            {
                LOG_E("Write register failed: %d", rc);
            }
        }
        // 模拟寄存器值变化
        rt_thread_mdelay(1000);
#endif

#if (TESTING_FUNCTION_CODE == 0x10)
        /**
         *      示例：写多个线圈或离散输出
         *      功能码：0x10
         *      地址：0
         *      写入值：write_regs（8个寄存器值）regs_value（0-100）
         *      返回值：MODBUS_OK 或错误码
         */

        rt_int16_t write_regs[16] = {0};

        for (index = 0; index < 8; index++)
        {
            write_regs[index] = regs_value++;
        }

        regs_value = (regs_value >= 100) ? 0 : regs_value;

        if (mtr_rtu_master_startup)
        {
            rc = mtr_rtu_mb_write_registers(rtu_master, 0, 8, write_regs);
            LOG_I("Master Write Multiple Register Address 0x0000 Length 8: %d", rc);
            if (rc >= MODBUS_OK)
            {
                LOG_I("Write register success.");
            }
            else
            {
                LOG_E("Write register failed: %d", rc);
            }
        }
        // 模拟寄存器值变化
        rt_thread_mdelay(1000);
#endif
    }
}

// 设备串口配置
static struct serial_configure serial_params_config(void)
{
    struct serial_configure config;
    // 设置波特率115200，数据位8，停止位1，无奇偶校验
    config.baud_rate = BAUD_RATE_115200;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.bufsz = 512;
    config.parity = PARITY_NONE;
    return config;
}

// RTU主站线程入口函数
static void mtr_rtu_master_thread_entry(void* params)
{
    // 获取RTU主站实例
    mtr_rtu_mb_t* app_master = params;

    // 创建互斥量
    mtr_rtu_master_mux = rt_mutex_create("mtr_mux", RT_IPC_FLAG_PRIO);

    // 配置串口参数
    struct serial_configure serial_config = serial_params_config();
    mtr_rtu_mb_rtu_set_serial_config(app_master, &serial_config);

    // 初始化串口设备
    mtr_rtu_mb_init(app_master, MODBUS_CORE_RTU, mtr_rtu_mb_port_rtdevice_create(MTR_RTU_UART_DEVICE_NAME));

    // 设置串口RTS和读写标志
    mtr_rtu_mb_rtu_set_oflag(app_master, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);

    // 建立RTU连接
    mtr_rtu_mb_connect(app_master);

    // 设备读写管理
    mtr_rtu_master_poll(app_master);
}

// RTU主站线程初始化函数
int mtr_rtu_master_thread(void)
{
    // 创建RTU主站线程
    rt_thread_t thread_id = rt_thread_create("mtr-master",                 // 线程名称
                                             mtr_rtu_master_thread_entry,  // 线程入口函数
                                             &mtr_rtu_master,              // 线程入口参数
                                             1024,                         // 线程栈大小
                                             13,                           // 线程优先级
                                             20                            // 时间片
    );

    if (thread_id != RT_NULL)
    {
        // 启动线程并标记已启动
        rt_thread_startup(thread_id);
        mtr_rtu_master_startup = RT_TRUE;
        return RT_EOK;
    }
    else
    {
        // 创建失败
        LOG_E("Failed to create RTU master thread!\n");
        return RT_ERROR;
    }
}

#if 0

MSH_CMD_EXPORT(mtr_rtu_master_thread, "RTU Master thread for MTR Modbus");

#endif