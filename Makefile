# Top-level build dispatcher for the STM8 project.
# Examples:
#   make                         Build application and bootloader
#   make TARGET=application     Build application only
#   make TARGET=bootloader      Build bootloader only
#   make TARGET=application CONFIG=debug
#   make clean

TARGET ?= all
CONFIG ?= release

BUILD_ARGS := CONFIG=$(CONFIG)
ifneq ($(strip $(MCU)),)
BUILD_ARGS += MCU=$(MCU)
endif

.PHONY: build all application bootloader clean binary flash help

.DEFAULT_GOAL := build

build: $(TARGET)

all: application bootloader

application:
	$(MAKE) -C application $(BUILD_ARGS) all

bootloader:
	$(MAKE) -C bootloader $(BUILD_ARGS) all

clean:
	$(MAKE) -C application $(BUILD_ARGS) clean
	$(MAKE) -C bootloader $(BUILD_ARGS) clean

binary:
	$(MAKE) -C application $(BUILD_ARGS) binary
	$(MAKE) -C bootloader $(BUILD_ARGS) binary

flash:
	$(MAKE) -C application $(BUILD_ARGS) flash

help:
	@echo "make [TARGET=all|application|bootloader] [CONFIG=release|debug]"
	@echo "make TARGET=application"
	@echo "make TARGET=bootloader CONFIG=debug"
	@echo "make clean"
