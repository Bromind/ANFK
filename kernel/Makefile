AS = /usr/bin/as
ASFLAGS = -g
CC = gcc
CFLAGS = -std=c99 -g -pg -Wall
RM = /bin/rm

SOURCE_ASM = $(wildcard *.s)
SOURCE_C = $(wildcard *.c)

OBJS = $(patsubst %s,%o,$(SOURCE_ASM))
OBJS += $(patsubst %c,%o,$(SOURCE_C))


SUBDIR = driver processManager

all: kernel.img

#FIXME
kernel.elf: $(OBJS) $(SUBDIR) kernel.ld
	ld $(OBJS) 

kernel.img: kernel.elf
	objcopy kernel.elf -O binary kernel.img

$(SUBDIR): 
	$(MAKE) -C $@

clean: 
	$(RM) $(OBJ)
