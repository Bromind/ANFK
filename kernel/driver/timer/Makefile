AS = /usr/bin/as
ASFLAGS = -g
CC = gcc
CFLAGS = -std=c99 -g -pg -Wall
RM = /bin/rm
OBJ = *.o

all: 
	$(AS) $(ASFLAGS) systemTimer.s -o systemTimer.o

clean: 
	$(RM) $(OBJ)
