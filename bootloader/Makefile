CC=		sdcc
OBJCOPY =	sdobjcopy
#SRCS=	./main.c
#SRCS= $(wildcard *.c)
SRC_DIR = .
# Find all .c files recursively under SOURCEDIR
SRCS := $(shell find $(SRC_DIR) -name '*.c')
INC_DIR := $(dir $(shell find $(SRC_DIR) -name '*.h'))
LIBS=	-lstm8 -mstm8
IDIR=	../include
ODIR=	../bin
CFLAGS=	$(addprefix -I,$(INC_DIR))
BUILD_DIR = ../bin/bootloader
EXECUTABLE = bootloader
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.rel, $(SRCS))
ASMS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.asm, $(SRCS))
LSTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.lst, $(SRCS))
SYMS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.sym, $(SRCS))
RSTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.rst, $(SRCS))

all: $(OBJS)
	mkdir -p $(BUILD_DIR)
#	$(CC) $(SRCS) -I$(CFLAGS) $(LIBS) --out-fmt-ihx -o$(ODIR)/
	echo $(SRCS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) --out-fmt-ihx -o $(BUILD_DIR)/$(EXECUTABLE).hex
	
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	@echo $(INC_DIR) 
	$(CC) $(CFLAGS) $(LIBS) -c $^ -o $@

binary:
	$(OBJCOPY) -I ihex $(BUILD_DIR)/$(EXECUTABLE).hex -O binary $(BUILD_DIR)/$(EXECUTABLE).bin
	
.PHONY: clean flash

clean:
	@echo Object files are: $(OBJS)
	@echo ASM files are: $(ASMS)
	@echo LST files are: $(LSTS)
	@echo SYM files are: $(SYMS)
	@echo RST files are: $(RSTS)
	rm -rf $(OBJS) $(ASMS) $(LSTS) $(SYMS) $(RSTS) $(BUILD_DIR)/$(EXECUTABLE).hex
	
flash:
	stm8flash -c stlinkv2 -pstm8s003f3 -w $(BUILD_DIR)/$(EXECUTABLE).hex
