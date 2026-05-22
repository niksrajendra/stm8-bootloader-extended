## STM8S
MCU     ?= stm8s003f3
FAMILY  ?= STM8S

ARCH     = stm8

# Configuration: debug or release (default: release)
CONFIG   ?= release

CC		= sdcc
AS		= sdasstm8
OBJCOPY =	sdobjcopy
#SRCS=	./main.c
#SRCS= $(wildcard *.c)
SRC_DIR = .
# Find all .c files recursively under SOURCEDIR
SRCS := $(shell find $(SRC_DIR) -name '*.c')
SRCasm := $(shell find $(SRC_DIR) -name '*.s')
INC_DIR := $(dir $(shell find $(SRC_DIR) -name '*.h'))
LIBS=	-lstm8 -mstm8
IDIR=	../include
ODIR=	../bin
ASFLAGS  = -plosgff
CFLAGS=	-m$(ARCH) -p$(MCU) -D$(FAMILY)
CFLAGS +=$(addprefix -I,$(INC_DIR))

# Configuration-specific flags
ifeq ($(CONFIG),debug)
    CFLAGS += -DDEBUG --debug
    CFLAGS += -O0
    BUILD_SUFFIX = _debug
else
    # Release configuration (default)
    #CFLAGS += -O2
    BUILD_SUFFIX =
endif

LDFLAGS  = -m$(ARCH) -l$(ARCH) --out-fmt-ihx
BUILD_DIR = ../bin/application$(BUILD_SUFFIX)
EXECUTABLE = application$(BUILD_SUFFIX)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.rel, $(SRCS))
OBJS += $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.rel, $(SRCasm))
ASMS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.asm, $(SRCS))
LSTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.lst, $(SRCS))
SYMS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.sym, $(SRCS))
RSTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.rst, $(SRCS))

all: $(OBJS)
	mkdir -p $(BUILD_DIR)
#	$(CC) $(SRCS) -I$(CFLAGS) $(LIBS) --out-fmt-ihx -o$(ODIR)/
	echo $(SRCS)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJS) -o $(BUILD_DIR)/$(EXECUTABLE).hex --code-loc 0x8C00
	@echo "Build complete: $(CONFIG) configuration"
	
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	@echo $(INC_DIR) 
	$(CC) $(CFLAGS) $(LIBS) -c $^ -o $@

$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	@echo $(INC_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

binary:
	$(OBJCOPY) -I ihex $(BUILD_DIR)/$(EXECUTABLE).hex -O binary $(BUILD_DIR)/$(EXECUTABLE).bin
	
.PHONY: clean flash debug release

debug:
	$(MAKE) CONFIG=debug all

release:
	$(MAKE) CONFIG=release all

clean:
	@echo Object files are: $(OBJS)
	@echo ASM files are: $(ASMS)
	@echo LST files are: $(LSTS)
	@echo SYM files are: $(SYMS)
	@echo RST files are: $(RSTS)
	@echo OBJSA files are: $(OBJSa)
	@echo src files are: $(SRCS)
	rm -rf $(OBJS) $(ASMS) $(LSTS) $(SYMS) $(RSTS) $(BUILD_DIR)/$(EXECUTABLE).hex
	
flash:
	stm8flash -c stlinkv2 -p$(MCU) -w $(BUILD_DIR)/$(EXECUTABLE).hex

dump_read:
	stm8flash -c stlinkv2 -p$(MCU) -r $(BUILD_DIR)/dump_read.hex
