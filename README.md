# STM8 Extended Bootloader

A UART bootloader and firmware project for STM8S microcontrollers. The bootloader receives firmware in fixed-size blocks, writes it to flash, and checks a CRC-8 before reporting success.

## Features

- Bootloader and application are built separately with SDCC.
- Application firmware is linked at `0x8C00`.
- Firmware updates use UART at 115200 baud, 8 data bits, no parity, and 1 stop bit.
- The included Python utility sends a binary firmware image and uses CRC-8 with polynomial `0x07`.
- MCU targets in the Makefiles: `stm8s003f3`, `stm8s005k6`, and `stm8s007c8`.

## Requirements

- GNU Make and an environment with `find` and `mkdir` (Git Bash, MSYS2, or WSL on Windows).
- SDCC with the STM8 tools (`sdcc`, `sdasstm8`, and `sdobjcopy`) on `PATH`.
- `stm8flash` for programming over an ST-Link V2.
- Python 3 and PySerial for the command-line firmware updater.

Install PySerial with:

```sh
python -m pip install pyserial
```

### Windows Git Bash

Git Bash provides the Unix-style shell commands used by the Makefiles, but it does not include GNU Make by default. Install Scoop if needed, then install Make from PowerShell:

```powershell
scoop install make
```

Close and reopen Git Bash, change to the repository root, and verify Make and the SDCC tools are available:

```sh
make --version
command -v sdcc
command -v sdasstm8
command -v sdobjcopy
```

Install SDCC for Windows separately if those tools are not found, and add its `bin` directory to your Windows `PATH`. Then build from the repository root using the same targets shown below, for example `make MCU=stm8s003f3`. Git Bash paths use `/e/...` instead of Windows drive paths such as `E:\...`.

## Build

The Makefiles default to `stm8s005k6`. Select the MCU that matches your hardware:

```sh
# Build both bootloader and application
make MCU=stm8s003f3

# Build just one target
make TARGET=bootloader MCU=stm8s003f3
make TARGET=application MCU=stm8s003f3

# Debug build
make CONFIG=debug MCU=stm8s003f3

# Remove generated build files
make clean MCU=stm8s003f3
```

Release HEX files are written to `bin/bootloader/` and `bin/application/`. Debug builds use `bin/bootloader_debug/` and `bin/application_debug/`. To produce binary images for the updater, run:

```sh
make binary MCU=stm8s003f3
```

The application binary is `bin/application/application.bin`.

## CI/CD

GitHub Actions runs Cppcheck, including its MISRA C:2012 addon, for the three MCU platform configurations currently defined in `platform.h` (`stm8s003f3`, `stm8s005k6`, and `stm8s007c8`). Analyzer findings fail the corresponding analysis job. Cppcheck's `misra-config` diagnostic is suppressed because the addon cannot resolve some MCU-specific hardware symbols; MISRA rule findings remain enabled. The build matrix attempts all six MCU entries listed in the Makefiles. The HEX and BIN files are available as workflow artifacts for 14 days. Pushing a version tag such as `v1.0.0` also creates a GitHub Release with the firmware files for each MCU attached.

## Program and Update

Program the bootloader onto a blank device using an ST-Link V2:

```sh
make TARGET=bootloader MCU=stm8s003f3
make -C bootloader flash MCU=stm8s003f3
```

To update application firmware, connect a voltage-compatible UART adapter with a common ground and crossed TX/RX. Hold PD4 low while resetting the target to enter bootloader mode, then run:

```sh
python utility/boot.py --port COM3 bin/application/application.bin
```

Replace `COM3` with the serial port for your adapter. The utility toggles DTR during startup; whether that resets the target depends on the adapter and board wiring. Keep the boot-entry pin low across that reset. On Linux, a port such as `/dev/ttyUSB0` can be used instead.

## Block-Size Compatibility

The firmware uses 64-byte blocks for STM8S003F3 and 128-byte blocks for STM8S005K6/STM8S007C8. The included Python and Qt utilities currently use 64-byte blocks. Therefore, the included updater currently matches the STM8S003F3 build; do not use it with the 128-byte firmware builds without first updating the utility to use the matching block size.

## Repository Layout

- `bootloader/`: bootloader firmware, peripheral drivers, and its Makefile.
- `application/`: example application and its Makefile.
- `utility/boot.py`: Python serial firmware updater.
- `utility/QT_GUI/`: Qt-based updater project.
- `hardware/`: development-board schematic and PCB files.