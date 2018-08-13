# 
# The MIT License (MIT)
# 
# Copyright (c) 2018 Anh Vo Tuan <votuananhs@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies
# or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
# AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# 
# build system for C project

PRO_DIR				:= .
FREERTOS_DIR		:= $(PRO_DIR)/../FreeRTOSv10.0.1/FreeRTOS/Source
FREERTOS_GCC_DIR 	:= $(FREERTOS_DIR)/portable/GCC/ARM_CM0
FREERTOS_HEAP_DIR 	:= $(FREERTOS_DIR)/portable/MemMang
# them line mơi
SRC_DIR				:= $(PRO_DIR)/driver/src
###
PROJ_NAME	?= stm32f0_cli_demo
BUILD_DIR 	:= $(PRO_DIR)/output

COMPILER_DIR 	:= C:/tools/compiler/7_2017-q4-major

INC_DIR		:= -I$(PRO_DIR)/driver/inc -I$(PRO_DIR)/lib -I$(PRO_DIR)/lib/cli/inc -I$(PRO_DIR)/app/inc -I$(FREERTOS_DIR)/include -I$(FREERTOS_GCC_DIR) -I$(COMPILER_DIR)/arm-none-eabi/include/sys
LINKER_FILE	:= $(PRO_DIR)/linker/stm32f0.ld

# them line moi 
FILE_TO_LINK	:= $(BUILD_DIR)/clock.o $(BUILD_DIR)/gpio.o $(BUILD_DIR)/interrupt.o $(BUILD_DIR)/main.o $(BUILD_DIR)/startup_ARMCM0.o $(BUILD_DIR)/systick.o $(BUILD_DIR)/usart.o
####
PREFIX_GCC_COMPILER	:= arm-none-eabi
CC				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-gcc
ASM				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-as
LD				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-ld
SIZE			:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-size
OBJCPY			:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-objcopy
NM				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-nm

# list of C language files:
CC_FILE			:= $(shell find $(PRO_DIR) -name *.c)
CC_FILE			+= $(FREERTOS_DIR)/tasks.c
CC_FILE			+= $(FREERTOS_DIR)/list.c
CC_FILE			+= $(FREERTOS_DIR)/queue.c
CC_FILE			+= $(FREERTOS_DIR)/event_groups.c
CC_FILE			+= $(FREERTOS_DIR)/stream_buffer.c
CC_FILE			+= $(FREERTOS_DIR)/timers.c
CC_FILE			+= $(FREERTOS_GCC_DIR)/port.c
CC_FILE			+= $(FREERTOS_HEAP_DIR)/heap_2.c

ASM_FILE		:= $(shell find $(PRO_DIR) -name *.s -or -name *.S)



BUILD_FILE		:= $(CC_FILE) $(ASM_FILE)
#OBJECT_FILE		:= $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(notdir $(BUILD_FILE)))))
DEPS 			:= $(OBJECT_FILE:.o=.d)

CC_OPT			:= -march=armv6-m -mcpu=cortex-m0 -c -g -mthumb $(INC_DIR) -DCLI_STAND_ALONE
ASM_OPT			:= -march=armv6-m -mcpu=cortex-m0 -c -mthumb --defsym __STARTUP_CLEAR_BSS=1 --defsym __STACK_SIZE=0x300 --defsym __HEAP_SIZE=0x800
LD_OPT			:= -T $(LINKER_FILE) -Map $(BUILD_DIR)/$(PROJ_NAME).map $(OBJECT_FILE) -L $(COMPILER_DIR)/arm-none-eabi/lib/thumb/v6-m -lc_nano -lnosys -L $(COMPILER_DIR)/lib/gcc/arm-none-eabi/7.2.1/thumb/v6-m -lgcc

.PHONY: all

all: clean build size


# # generate object file of c file
# ifndef generate_c_rule
# define generate_c_rule
# $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(notdir $(1))))): $(1)
# 	@echo Processing $$<
# 	$(CC) $(CC_OPT) $$< -o $$@
# endef
# endif # generate_c_rule

# # generate object file of c file
# ifndef generate_asm_rule
# define generate_asm_rule
# $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(notdir $(1))))): $(1)
# 	@echo Processing $$<
# 	$(ASM) $(ASM_OPT) $$< -o $$@
# endef
# endif # generate_asm_rule

# #generate all c and asm file of project
# $(foreach file,$(CC_FILE),$(eval $(call generate_c_rule,$(file))))
# $(foreach file,$(ASM_FILE),$(eval $(call generate_asm_rule,$(file))))
$(BUILD_DIR)/clock.o: $(SRC_DIR)/clock.c
	@echo "compile $(SRC_DIR)/clock.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/clock.c -o $(BUILD_DIR)/clock.o

$(BUILD_DIR)/gpio.o: $(SRC_DIR)/gpio.c
	@echo "compile $(SRC_DIR)/gpio.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/gpio.c -o $(BUILD_DIR)/gpio.o

$(BUILD_DIR)/interrupt.o: $(SRC_DIR)/interrupt.c
	@echo "compile $(SRC_DIR)/interrupt.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/interrupt.c -o $(BUILD_DIR)/interrupt.o

$(BUILD_DIR)/main.o: $(PRO_DIR)/app/src/main.c
	@echo "compile $(PRO_DIR)/app/src/main.c file"
	$(CC) $(CC_OPT) $(PRO_DIR)/app/src/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/startup_ARMCM0.o: $(PRO_DIR)/app/src/startup_ARMCM0.S
	@echo "compile $(PRO_DIR)/app/src/startup_ARMCM0.S file"
	$(ASM) $(ASM_OPT) $(PRO_DIR)/app/src/startup_ARMCM0.s -o $(BUILD_DIR)/startup_ARMCM0.o

$(BUILD_DIR)/systick.o: $(SRC_DIR)/systick.c
	@echo "compile $(SRC_DIR)/systick.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/systick.c -o $(BUILD_DIR)/systick.o

$(BUILD_DIR)/usart.o: $(SRC_DIR)/usart.c
	@echo "compile $(SRC_DIR)/usart.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/usart.c -o $(BUILD_DIR)/usart.o

size: $(BUILD_DIR)/$(PROJ_NAME).elf
	@echo "Size area memories in binary image $(BUILD_DIR)/$(PROJ_NAME).elf"
	$(SIZE) $(BUILD_DIR)/$(PROJ_NAME).elf

d_size: $(BUILD_DIR)/$(PROJ_NAME).elf
	$(NM) --demangle --print-size --size-sort --reverse-sort -S $(BUILD_DIR)/$(PROJ_NAME).elf

show_flag:
	@echo $(COMPILER_DIR)
	@echo $(CC_FILE)
	@echo $(ASM_FILE)
	@echo $(OBJECT_FILE)
	@echo $(CC_OPT)
	@echo $(ASM_OPT)
	@echo $(LD_OPT)

hex: $(BUILD_DIR)/$(PROJ_NAME).elf
	$(OBJCPY) -O ihex $(BUILD_DIR)/$(PROJ_NAME).elf $(BUILD_DIR)/$(PROJ_NAME).hex

# build: $(OBJECT_FILE)
# 	@echo "Linking object files to create new binary image $(BUILD_DIR)/$(PROJ_NAME).elf"
# 	$(LD) $(LD_OPT) -o $(BUILD_DIR)/$(PROJ_NAME).elf
build: $(FILE_TO_LINK) $(LINKER_FILE)
	@echo "link object files to create new binary image ($(PROJ_NAME).elf)"
	$(LD) $(LD_OPT) -o $(BUILD_DIR)/$(PROJ_NAME).elf

clean:
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

-include $(DEPS)


