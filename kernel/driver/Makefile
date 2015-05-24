AS = /usr/bin/as
ASFLAGS = #-g
CC = gcc
CFLAGS = -std=c99 -Wall #-g -pg
RM = /bin/rm

SOURCE_ASM = $(wildcard *.s)
SOURCE_C = $(wildcard *.c)

OBJS = $(patsubst %s,%o,$(SOURCE_ASM))
OBJS += $(patsubst %c,%o,$(SOURCE_C))

SUBDIR = gpio drawing frameBuffer mailbox timer

all: $(SUBDIR)

$(SUBDIR): force_look
	$(MAKE) -C $@

force_look: 
	true

clean: 
	-for d in $(SUBDIR); do (cd $$d ; $(MAKE) clean); done
