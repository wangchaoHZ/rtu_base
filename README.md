# RTU Base - Modbus RTU Library

A C library implementing Modbus RTU protocol for embedded systems, specifically designed for RT-Thread RTOS.

## Project Structure

- `mtr_rtu_mb_base.c/h` - Core Modbus base functionality and public API
- `mtr_rtu_mb_core.c/h` - RTU protocol implementation (CRC, framing)
- `mtr_rtu_mb_utils.c/h` - Utility functions (CRC16, data conversion)
- `mtr_rtu_mb_port_rtthread.c/h` - RT-Thread platform port (UART, threading)
- `mtr_rtu_virtualIO.c/h` - Template for user I/O implementation
- `mtr_rtu_mb_master_app.c` - Example master application
- `mtr_rtu_mb_slave_app.c` - Example slave application
- `config_mtr_rtu_mb.h` - Configuration settings

## Code Conventions

### Formatting
- **Formatter**: clang-format (configured in `.clang-format`)
- **Indentation**: 4 spaces, no tabs
- **Line Length**: 120 characters maximum
- **Braces**: Linux kernel style (opening brace on same line for functions)

To format code:
```bash
clang-format -i -style=file <file.c>
```

### Naming Conventions

#### Header Guards
- Format: `<FILENAME>_H` (without leading/trailing underscores)
- Example: `MTR_RTU_MB_BASE_H`

#### Functions
- Public API: `mtr_rtu_mb_<action>()` format
  - Examples: `mtr_rtu_mb_connect()`, `mtr_rtu_mb_read_registers()`
- Internal functions: `static` with descriptive names
- Platform-specific: Prefixed with `_mtr_rtu_mb_`

#### Types
- Structs: `typedef struct _name name_t;`
- Example: `mtr_rtu_mb_t`, `mtr_rtu_mb_port_t`

#### Constants and Enums
- All uppercase with underscores: `MODBUS_OK`, `MTR_MODBUS_MAX_PDU_LENGTH`
- Prefixed with `MODBUS_` or `MTR_MODBUS_` for scope clarity

#### Variables
- Local variables: lowercase with underscores
- Struct members: lowercase with underscores

### File Naming
- All lowercase with underscores
- Prefix: `mtr_rtu_` for library files
- Pattern: `mtr_rtu_<module>_<component>.c/h`

## Building

This library is designed to be integrated into RT-Thread projects. Include the necessary files in your RT-Thread project and configure via `config_mtr_rtu_mb.h`.

### Configuration
Edit `config_mtr_rtu_mb.h` to configure:
- UART device name
- Slave address
- Thread parameters (stack size, priority, ticks)
- RT-Thread specific settings

## Virtual I/O Implementation

The library requires user implementation of I/O handlers declared in `mtr_rtu_virtualIO.h`:

- `mtr_rtu_read_hold_regs()` - Read holding registers
- `mtr_rtu_write_hold_regs()` - Write holding registers
- `mtr_rtu_read_input_regs()` - Read input registers
- `mtr_rtu_read_hold_coils()` - Read holding coils
- `mtr_rtu_read_input_coils()` - Read input coils
- `mtr_rtu_write_hold_coils()` - Write holding coils

See `mtr_rtu_virtualIO.c` for a template implementation (currently disabled with `#if 0`).

## API Overview

### Master Mode Functions
```c
// Read operations
int mtr_rtu_mb_read_registers(mtr_rtu_mb_t* smb, int addr, int num, uint16_t* read_data);
int mtr_rtu_mb_read_input_registers(mtr_rtu_mb_t* smb, int addr, int num, uint16_t* read_data);
int mtr_rtu_mb_read_bits(mtr_rtu_mb_t* smb, int addr, int num, uint8_t* read_data);
int mtr_rtu_mb_read_input_bits(mtr_rtu_mb_t* smb, int addr, int num, uint8_t* read_data);

// Write operations
int mtr_rtu_mb_write_register(mtr_rtu_mb_t* smb, int addr, int write_value);
int mtr_rtu_mb_write_registers(mtr_rtu_mb_t* smb, int addr, int num, uint16_t* write_data);
int mtr_rtu_mb_write_bit(mtr_rtu_mb_t* smb, int addr, int write_status);
int mtr_rtu_mb_write_bits(mtr_rtu_mb_t* smb, int addr, int num, uint8_t* write_data);
```

### Slave Mode Functions
```c
// Wait for and handle requests
int mtr_rtu_mb_slave_wait_handle(mtr_rtu_mb_t* smb, mtr_rtu_mb_slave_callback_t callback, int32_t waittime);
```

### Connection Management
```c
int mtr_rtu_mb_connect(mtr_rtu_mb_t* smb);
int mtr_rtu_mb_disconnect(mtr_rtu_mb_t* smb);
```

## Return Codes

- `MODBUS_OK` (0) - Success
- `MODBUS_FAIL` (-1) - General failure
- `MODBUS_TIMEOUT` (-2) - Operation timeout
- `MODBUS_FAIL_CHECK` (-7) - CRC check failed
- `MODBUS_ERROR_CONTEXT` (-10) - Invalid context

## License

Copyright 2024 wang chao, All Rights Reserved.

## Changelog

### Recent Changes
- Added CRC16 lookup table implementation
- Standardized header guards (removed leading underscores)
- Applied consistent code formatting via clang-format
- Made internal helper functions static where appropriate
- Added comprehensive documentation
