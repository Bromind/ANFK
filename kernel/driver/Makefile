AS = /usr/bin/as
ASFLAGS = -g
CC = gcc
CFLAGS = -std=c99 -g -pg -Wall
RM = /bin/rm
OBJ = *.o

SUBDIR = drawing frameBuffer gpio mailbox systemTimer

.PHONY: subdirs $(SUBDIR)

subdirs: $(SUBDIR)

$(SUBDIR):
	$(MAKE) -C $@


