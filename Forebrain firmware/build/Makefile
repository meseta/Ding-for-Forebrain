###############################################################################
# Copyright (c) 2011, Universal Air Ltd. All rights reserved.                 #
# Source and binaries are released under the BSD 3-Clause license             #
# See license.txt files for the text of the license                           #
###############################################################################
 
CORTEX = cortex-m3
MPU = LPC13xx

COMPILER = arm-none-eabi-
CC = $(COMPILER)gcc
AS = $(COMPILER)gcc
LD = $(COMPILER)gcc
SIZE = $(COMPILER)size
OBJCOPY = $(COMPILER)objcopy
OBJDUMP = $(COMPILER)objdump
CHKSM = checksum
PRJPATH = ./build

LD_SCRIPT = $(PRJPATH)/linker.ld
INCLUDE = -I $(PRJPATH)
INCLUDE += -I $(PRJPATH)/../
VPATH = $(PRJPATH)


OBJS = $(PRJPATH)/main.o
OBJS += $(PRJPATH)/startup.o
OBJS += $(PRJPATH)/core_cm3.o
OBJS += $(PRJPATH)/uafunc.o

### Compiler stuff ###

CFLAGS  = -c -g -Os $(INCLUDE) -Wall -mthumb -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CORTEX) -DTARGET=$(MPU) -fno-builtin
ASFLAGS = -c -g -Os $(INCLUDE) -Wall -mthumb -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CORTEX) -D__ASSEMBLY__ -x assembler-with-cpp
LDFLAGS = -nostartfiles -mthumb -mcpu=$(CORTEX) -Wl,--gc-sections
OCFLAGS = --strip-unneeded

all: firmware

$(PRJPATH)/%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

$(PRJPATH)/%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<

firmware: $(OBJS)
	-@echo "*** Compiling... ***"
	$(LD) $(LDFLAGS) -T $(LD_SCRIPT) -o $(PRJPATH)/firmware.elf $(OBJS) -lm
	-@echo ""
	-@echo "*** Converting... ***"
	$(OBJCOPY) $(OCFLAGS) -O binary $(PRJPATH)/firmware.elf firmware.bin
	-@echo ""
	-@echo "*** Producint disassembly listing... ***"
	$(OBJDUMP) -d -S $(PRJPATH)/main.o > $(PRJPATH)/disassembly.lst
	-@echo ""
	-@echo "*** Adding Checksum... ***"
	$(CHKSM) firmware.bin
	-@echo ""
	-@echo "*** Getting Size information... ***"
	$(SIZE) $(PRJPATH)/firmware.elf
	-@echo ""
	-@echo "*** Compile complete! ***"
clean:
	rm -f $(OBJS) $(PRJPATH)/firmware.elf firmware.bin $(PRJPATH)/disassembly.lst
