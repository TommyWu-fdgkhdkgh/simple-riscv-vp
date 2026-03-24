ARCH = riscv64-unknown-elf
CC = $(ARCH)-gcc
LD = $(ARCH)-ld
OBJCOPY = $(ARCH)-objcopy
OBJDUMP := $(ARCH)-objdump
READELF := $(ARCH)-readelf
SIZE := $(ARCH)-size

SDIR = ./
BDIR := build
S_SRCS = $(wildcard $(SDIR)/*.S)
C_SRCS = $(wildcard $(SDIR)/*.c)
S_OBJS = $(S_SRCS:$(SDIR)/%.S=$(BDIR)/%_asm.o)
C_OBJS = $(C_SRCS:$(SDIR)/%.c=$(BDIR)/%.o)

MARCH ?= rv32i_zicsr
MABI ?= ilp32
CFLAGS = -Wall -g -O0 -march=${MARCH} -mabi=${MABI} -ffreestanding
SFLAGS = -g -march=${MARCH} -mabi=${MABI}
LDFLAGS = -Tsimple.ld -m elf32lriscv

.PHONY: all
all: ${BDIR}/simple.elf

${BDIR}/simple.elf: $(S_OBJS) simple.ld $(C_OBJS)
	$(LD) ${LDFLAGS} -o $@ $(S_OBJS) $(C_OBJS)
	@ ${OBJDUMP} -S $@ > $@.dis
	@ ${READELF} -S $@ > $@.info
	@ ${SIZE} $@

$(BDIR)/%.o: $(SDIR)/%.c
	@ mkdir -p $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/%_asm.o: $(SDIR)/%.S
	@ mkdir -p $(BDIR)
	$(CC) $(SFLAGS) -c $< -o $@

clean:
	rm -rf ${BDIR}
