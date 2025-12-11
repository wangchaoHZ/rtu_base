# Changelog

## [Unreleased] - Standardization and Documentation

### Added
- `.clang-format` - Automated C code formatting configuration
- `.editorconfig` - Cross-editor consistency settings
- `.gitignore` - Build artifacts and IDE files exclusion
- `README.md` - Comprehensive project documentation including:
  - Project structure overview
  - API reference and usage examples
  - Coding conventions and naming standards
  - Build and configuration instructions
  - Virtual I/O implementation guide
- `CHANGELOG.md` - This file for tracking changes

### Changed
- **Code Formatting**: Applied clang-format to all `.c` and `.h` files
  - Consistent indentation (4 spaces)
  - Standardized brace placement
  - Better alignment and readability
  - Net reduction of ~280 lines through cleaner formatting

- **Header Guards**: Standardized across all headers
  - Format: `FILENAME_H` (removed leading underscores per C++ best practices)
  - Updated closing `#endif` comments to match guard names
  - Changed guards in: `config_mtr_rtu_mb.h`, `mtr_rtu_mb.h`, `mtr_rtu_mb_base.h`, 
    `mtr_rtu_mb_core.h`, `mtr_rtu_mb_port_rtthread.h`, `mtr_rtu_mb_utils.h`, `mtr_rtu_virtualIO.h`

- **Function Visibility**: Made `mtr_rtu_mb_context_check()` static
  - This function was only used internally within `mtr_rtu_mb_base.c`
  - Now properly scoped to prevent external linkage

- **Documentation**: Enhanced `mtr_rtu_virtualIO.c` comments
  - Clarified that the file contains template implementations
  - Explained that the `#if 0` block is intentional for user customization
  - Added guidance on how to implement the I/O handlers

### Fixed
- **Build Dependency**: Removed external dependency on `u_utilits.h`
  - Added inline Modbus CRC16 lookup table in `mtr_rtu_mb_utils.c`
  - Ensures project can build standalone without external dependencies
  - Standard 256-entry CRC16 table for Modbus protocol

### Removed
- `#include "u_utilits.h"` from `mtr_rtu_mb_utils.c`
- Duplicate `#include <stdint.h>` from `mtr_rtu_mb_utils.c`

### Code Quality Metrics
- **Files Changed**: 18
- **Lines Added**: 1,124 (mostly documentation and CRC table)
- **Lines Removed**: 1,088 (formatting improvements and removed dependency)
- **Net Change**: +36 lines (mostly from new documentation files)
- **Formatting Impact**: ~280 lines reduced in existing code

### Naming Convention Standards Established
- **Header Guards**: `FILENAME_H` (e.g., `MTR_RTU_MB_BASE_H`)
- **Public Functions**: `mtr_rtu_mb_<action>()` (e.g., `mtr_rtu_mb_connect()`)
- **Internal Functions**: `_mtr_rtu_mb_<name>()` or `static` where appropriate
- **Types**: `name_t` suffix (e.g., `mtr_rtu_mb_t`, `mtr_rtu_mb_port_t`)
- **Constants/Enums**: `PREFIX_NAME` in SCREAMING_SNAKE_CASE (e.g., `MODBUS_OK`)
- **Files**: lowercase with underscores (e.g., `mtr_rtu_mb_base.c`)

### Preserved
- ✅ All public API functions unchanged
- ✅ Template code in `#if 0` blocks kept as user reference
- ✅ Example applications (master and slave) intact
- ✅ Configuration options in `config_mtr_rtu_mb.h` unchanged
- ✅ No behavioral modifications
- ✅ Binary compatibility maintained

### Migration Notes
- No code changes required for existing users
- Optional: Run `clang-format -i -style=file *.c *.h` on custom implementations to match style
- Optional: Update custom header guards to new format

### Technical Details
- Formatter: clang-format v18.1.3
- Style Base: LLVM with customizations
- Line Length: 120 characters maximum
- Indentation: 4 spaces, no tabs
- Brace Style: Linux kernel style

---

For complete documentation, see [README.md](README.md).
