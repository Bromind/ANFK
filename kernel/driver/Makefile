AS = /usr/bin/as
ASFLAGS = -g
CC = gcc
CFLAGS = -std=c99 -g -pg -Wall
RM = /bin/rm
OBJ = *.o

SUBDIR = drawing frameBuffer gpio mailbox timer

.PHONY: subdirs $(SUBDIR)

all: subdirs

subdirs: $(SUBDIR)

$(SUBDIR):
	$(MAKE) -C $@


