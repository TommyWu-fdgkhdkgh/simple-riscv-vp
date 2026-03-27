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
D_C_SRCS = $(wildcard $(SDIR)/dhrystone/*.c)
S_OBJS = $(S_SRCS:$(SDIR)/%.S=$(BDIR)/%_asm.o)
C_OBJS = $(C_SRCS:$(SDIR)/%.c=$(BDIR)/%.o)
D_C_OBJS = $(D_C_SRCS:$(SDIR)/dhrystone/%.c=$(BDIR)/dhrystone/%.o)
H_FILE = $(wildcard $(SDIR)/*.h)
D_H_FILE = $(wildcard $(SDIR)/dhrystone/*.h)

MARCH ?= rv32i_zicsr
MABI ?= ilp32
CFLAGS = -Wall -g -O0 -march=${MARCH} -mabi=${MABI} -ffreestanding -I${SDIR}
DHRY_CFLAGS = -DTIMES -DNOENUM -DDHRY_ITERS=6000
SFLAGS = -g -march=${MARCH} -mabi=${MABI}
LDFLAGS = -Tsimple.ld -nostdlib -nodefaultlibs -march=${MARCH} -mabi=${MABI}
LIBS = -lgcc

.PHONY: all
all: ${BDIR}/simple.elf

${BDIR}/simple.elf: $(S_OBJS) simple.ld $(C_OBJS) $(D_C_OBJS)
	$(CC) ${LDFLAGS}  $(S_OBJS) $(C_OBJS) $(D_C_OBJS) $(LIBS) -o $@
	@ ${OBJDUMP} -S $@ > $@.dis
	@ ${READELF} -S $@ > $@.info
	@ ${SIZE} $@

$(BDIR)/dhrystone/%.o: $(SDIR)/dhrystone/%.c $(D_H_FILE)
	@ mkdir -p $(BDIR)/dhrystone
	$(CC) $(CFLAGS) $(DHRY_CFLAGS) -c $< -o $@

$(BDIR)/%.o: $(SDIR)/%.c $(H_FILE)
	@ mkdir -p $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/%_asm.o: $(SDIR)/%.S
	@ mkdir -p $(BDIR)
	$(CC) $(SFLAGS) -c $< -o $@

clean:
	rm -rf ${BDIR}
