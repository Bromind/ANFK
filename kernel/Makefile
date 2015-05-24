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

SUBDIR = driver processManager mem syscall ../utils fs

all: $(KERNEL_IMG) $(LIST)

#FIXME
$(KERNEL_ELF): $(OBJS) $(SUBDIR) $(LINKER)
	ld --no-undefined main.o  driver/frameBuffer/frameBuffer.o driver/drawing/drawing.o driver/mailbox/mailbox.o kernelStart.o driver/gpio/gpio.o driver/timer/systemTimer.o initFrameBuffer.o init.o initSubsystems.o processManager/processManager.o processManager/transfer.o mem/allocationTable.o syscall/alloc.o syscall/file.o syscall/exec.o fs/fs.o ../utils/linkedList.o ../utils/maths/div.o ../utils/string/string.o mem/freeMemory.o -Map kernel.map -o kernel.elf -T kernel.ld
$(KERNEL_IMG): $(KERNEL_ELF)
	objcopy $^ -O binary $@

$(LIST): $(KERNEL_ELF) 
	objdump -d $(KERNEL_ELF) > $@

$(SUBDIR): force_look
	$(MAKE) -C $@

force_look: 
	true

clean: 
	-$(RM) $(OBJS)
	-$(RM) $(KERNEL_MAP)
	-$(RM) $(KERNEL_ELF)
	-$(RM) $(KERNEL_IMG)
	-$(RM) $(LIST)
	-for d in $(SUBDIR); do (cd $$d ; $(MAKE) clean); done
