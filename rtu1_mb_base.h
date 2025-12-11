#ifndef _RTU1_BASE_H_
#define _RTU1_BASE_H_

#include "stdint.h"
#include "stdio.h"
#include "string.h"

/* function codes */
enum functionCode
{
    MODBUS_FC_READ_HOLDING_COILS = 0x01,
    MODBUS_FC_READ_INPUTS_COILS = 0x02,
    MODBUS_FC_READ_HOLDING_REGISTERS = 0x03,
    MODBUS_FC_READ_INPUT_REGISTERS = 0x04,
    MODBUS_FC_WRITE_SINGLE_COIL = 0x05,
    MODBUS_FC_WRITE_SINGLE_REGISTER = 0x06,
    MODBUS_FC_READ_EXCEPTION_STATUS = 0x07,
    MODBUS_FC_WRITE_MULTIPLE_COILS = 0x0F,
    MODBUS_FC_WRITE_MULTIPLE_REGISTERS = 0x10,
    MODBUS_FC_REPORT_SLAVE_ID = 0x11,
    MODBUS_FC_MASK_WRITE_REGISTER = 0x16,
    MODBUS_FC_WRITE_AND_READ_REGISTERS = 0x17,
};

enum returnCode
{
    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE = -0x83,
    MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS = -0x82,
    MODBUS_EXCEPTION_ILLEGAL_FUNCTION = -0x81,
    MODBUS_EXCEPTION = -0x80,
    MODBUS_ERROR_CONTEXT = -0x0A,
    MODBUS_ERROR_WAIT = -9,
    MODBUS_ERROR_READ = -8,
    MODBUS_FAIL_CHECK = -7,
    MODBUS_FAIL_ADRR = -6,
    MODBUS_FAIL_HANDLE = -5,
    MODBUS_FAIL_CONFIRM = -4,
    MODBUS_FAIL_REQUEST = -3,
    MODBUS_TIMEOUT = -2,
    MODBUS_FAIL = -1,
    MODBUS_OK = 0
};

enum waitCode
{
    MODBUS_WAIT_FOREVER = -1,
    MODBUS_WAIT_NO = 0
};

enum coreType
{
    MODBUS_CORE_NONE = 0,
    MODBUS_CORE_RTU = 1,
    MODBUS_CORE_TCP = 2
};

enum portType
{
    MODBUS_PORT_NONE = 0,
    MODBUS_PORT_DEVICE = 1,
    MODBUS_PORT_SOCKET = 2
};

enum deviceType
{
    MODBUS_DEVICE_NONE = 0,
    MODBUS_DEVICE_SLAVE = 1,
    MODBUS_DEVICE_MASTER = 2
};

enum mbMagic
{
    MODBUS_MAGIC = 0x4243424D,       //"MBCB"
    MODBUS_CORE_MAGIC = 0x4343424D,  //"MBCC"
    MODBUS_PORT_MAGIC = 0x5050424D,  //"MBPP"
};

#define RTU1_BROADCAST_ADDRESS 0

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define RTU1_MAX_READ_BITS  2000
#define RTU1_MAX_WRITE_BITS 1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define RTU1_MAX_READ_REGISTERS     125
#define RTU1_MAX_WRITE_REGISTERS    123
#define RTU1_MAX_WR_WRITE_REGISTERS 121
#define RTU1_MAX_WR_READ_REGISTERS  125

/* The size of the MODBUS PDU is limited by the size constraint inherited from
 * the first MODBUS implementation on Serial Line network (max. RS485 ADU = 256
 * bytes). Therefore, MODBUS PDU for serial line communication = 256 - Server
 * address (1 byte) - CRC (2 bytes) = 253 bytes.
 */
#define RTU1_MAX_PDU_LENGTH 253

/* Consequently:
 * - RTU MODBUS ADU = 253 bytes + Server address (1 byte) + CRC (2 bytes) = 256
 *   bytes.
 * - TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes.
 * so the maximum of both backend in 260 bytes. This size can used to allocate
 * an array of bytes to store responses and it will be compatible with the two
 * backends.
 */
#define RTU1_MAX_ADU_LENGTH 260

/*
 *bit max 2000
 *reg max 125
 */
#define RTU1_MAX_SWAP_LENGTH 250  // data length

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
 */
#define _MIN_REQ_LENGTH 12

#define _REPORT_SLAVE_ID 180

#define _RTU1_EXCEPTION_RSP_LENGTH 5

typedef struct _rtu1_mb rtu1_mb_t;
typedef struct _rtu1_mb_core rtu1_mb_core_t;  // mb core (mb-rtu,mb-tcp)
typedef struct _rtu1_mb_port rtu1_mb_port_t;  // mb port (rtthread  ,linux ,win32) serial socket

struct _rtu1_mb_core
{
    const uint32_t magic;
    const uint16_t type;
    const uint16_t len_header;
    const uint16_t len_checksum;
    const uint16_t len_adu_max;
    int (*build_request_header)(rtu1_mb_t* smb, uint8_t* buff, int slave, int fun, int reg, int num);
    int (*build_response_header)(rtu1_mb_t* smb, uint8_t* buff, int slave, int fun);
    int (*check_send_pre)(rtu1_mb_t* smb, uint8_t* buff, int length);
    int (*check_wait_request)(rtu1_mb_t* smb, uint8_t* buff, int length);
    int (*check_wait_response)(rtu1_mb_t* smb, uint8_t* buff, int length);
};

struct _rtu1_mb_port
{
    const uint32_t magic;
    const uint32_t type;
    int (*open)(rtu1_mb_t* smb);
    int (*close)(rtu1_mb_t* smb);
    int (*read)(rtu1_mb_t* smb, uint8_t* data, uint16_t length);
    int (*write)(rtu1_mb_t* smb, uint8_t* data, uint16_t length);
    int (*flush)(rtu1_mb_t* smb);
    int (*wait)(rtu1_mb_t* smb, int timeout);
};

struct _rtu1_mb
{
    uint32_t mb_magic;
    uint16_t device_mode;
    uint8_t slave_addr;
    uint8_t debug_level;

    uint16_t transfer_id;
    uint16_t protocol_id;

    int status;
    int error_code;
    uint32_t timeout_frame;                         // 帧超时时间ms
    uint32_t timeout_byte;                          // 字节超时时间ms
    uint8_t read_buff[RTU1_MAX_ADU_LENGTH];   // mb读缓冲区
    uint8_t write_buff[RTU1_MAX_ADU_LENGTH];  // mb写缓冲区
    rtu1_mb_core_t* core;                        // mb core (mb-rtu,mb-tcp)
    rtu1_mb_port_t* port;                        // mb port (rtthread  ,linux ,win32) serial socket
};

int _rtu1_mb_init(rtu1_mb_t* smb);
int _rtu1_mb_debug(rtu1_mb_t* smb, int level, const char* fmt, ...);

#define rtu1_mb_debug(smb, ...)       _rtu1_mb_debug(smb, 3, __VA_ARGS__)
#define rtu1_mb_debug_info(smb, ...)  _rtu1_mb_debug(smb, 2, __VA_ARGS__)
#define rtu1_mb_debug_error(smb, ...) _rtu1_mb_debug(smb, 1, __VA_ARGS__)

/* base api */
int rtu1_mb_connect(rtu1_mb_t* smb);
int rtu1_mb_disconnect(rtu1_mb_t* smb);
int rtu1_mb_write(rtu1_mb_t* smb, uint8_t* data, uint16_t length);
int rtu1_mb_read(rtu1_mb_t* smb, uint8_t* data, uint16_t length);
int rtu1_mb_flush(rtu1_mb_t* smb);
int rtu1_mb_wait(rtu1_mb_t* smb, int timeout);
int rtu1_mb_error_recovery(rtu1_mb_t* smb);
int rtu1_mb_error_exit(rtu1_mb_t* smb, int code);

int rtu1_mb_set_frame_timeout(rtu1_mb_t* smb, int timeout_ms);
int rtu1_mb_set_byte_timeout(rtu1_mb_t* smb, int timeout_ms);
int rtu1_mb_set_slave(rtu1_mb_t* smb, int slave);
int rtu1_mb_set_debug(rtu1_mb_t* smb, int level);

/* master mode api */
/* master start request */
int rtu1_mb_start_request(rtu1_mb_t* smb, uint8_t* request, int function, int addr, int num, void* write_data);
/* master wait for confirmation message */
int rtu1_mb_wait_confirm(rtu1_mb_t* smb, uint8_t* response);
/* master handle confirmation message */
int rtu1_mb_handle_confirm(rtu1_mb_t* smb, uint8_t* request, uint16_t request_len, uint8_t* response, uint16_t response_len, void* read_data);
/* master read */
int rtu1_mb_read_bits(rtu1_mb_t* smb, int addr, int num, uint8_t* read_data);
int rtu1_mb_read_input_bits(rtu1_mb_t* smb, int addr, int num, uint8_t* read_data);
int rtu1_mb_read_registers(rtu1_mb_t* smb, int addr, int num, uint16_t* read_data);
int rtu1_mb_read_input_registers(rtu1_mb_t* smb, int addr, int num, uint16_t* read_data);
/* master write */
int rtu1_mb_write_bit(rtu1_mb_t* smb, int addr, int write_status);
int rtu1_mb_write_register(rtu1_mb_t* smb, int addr, int write_value);
int rtu1_mb_write_bits(rtu1_mb_t* smb, int addr, int num, uint8_t* write_data);
int rtu1_mb_write_registers(rtu1_mb_t* smb, int addr, int num, uint16_t* write_data);
/* master write and read */
int rtu1_mb_mask_write_register(rtu1_mb_t* smb, int addr, uint16_t and_mask, uint16_t or_mask);
int rtu1_mb_write_and_read_registers(rtu1_mb_t* smb, int write_addr, int write_nb, uint16_t* src, int read_addr, int read_nb, uint16_t* dest);

/* slave callback */
typedef int (*rtu1_mb_slave_callback_t)(rtu1_mb_t* smb, int function_code, int addr, int num, void* read_write_data);

/* slave mode api */
/* slave wait query data */
int rtu1_mb_slave_wait(rtu1_mb_t* smb, uint8_t* request, int32_t waittime);
/* slave handle query data for callback */
int rtu1_mb_slave_handle(rtu1_mb_t* smb, uint8_t* request, uint16_t request_len, rtu1_mb_slave_callback_t slave_callback);
/* slave wait and handle query for callback */
int rtu1_mb_slave_wait_handle(rtu1_mb_t* smb, rtu1_mb_slave_callback_t slave_callback, int32_t waittime);

#endif /* _RTU1_BASE_H_ */
