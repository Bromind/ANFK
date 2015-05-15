AS = /usr/bin/as
ASFLAGS = #-g
CC = gcc
CFLAGS = -std=c99 -Wall #-g -pg
RM = /bin/rm

SOURCE_ASM = $(wildcard *.s)
SOURCE_C = $(wildcard *.c)

OBJS = $(patsubst %s,%o,$(SOURCE_ASM))
OBJS += $(patsubst %c,%o,$(SOURCE_C))

KERNEL_MAP = kernel.map
KERNEL_ELF = kernel.elf
KERNEL_IMG = kernel.img

LINKER = kernel.ld

LIST = kernel.list

SUBDIR = driver processManager mem

all: $(KERNEL_IMG) $(LIST)

#FIXME
$(KERNEL_ELF): $(OBJS) $(SUBDIR) $(LINKER)
	ld --no-undefined $(wildcard driver/*/*.o) initFrameBuffer.o kernelStart.o main.o -Map $(KERNEL_MAP) -o $@ -T $(LINKER) 
$(KERNEL_IMG): $(KERNEL_ELF)
	objcopy $^ -O binary $@

$(LIST): $(KERNEL_ELF) 
	objdump -d $(KERNEL_ELF) > $@

$(SUBDIR): 
	$(MAKE) -C $@

clean: 
	-$(RM) $(OBJS)
	-$(RM) $(KERNEL_MAP)
	-$(RM) $(KERNEL_ELF)
	-$(RM) $(KERNEL_IMG)
	-$(RM) $(LIST)
